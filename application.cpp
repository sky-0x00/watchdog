#include <cassert>
#include <algorithm>
#include "application.h"

#ifdef max
	#undef max
#endif

//---------------------------------------------------------------------------------------------------------------------------------------------
process::snapshot::snapshot(
) :
	_handle(Winapi::Toolhelp32::CreateSnapshot(TH32CS_SNAPPROCESS))
{
	assert(_handle);
}
process::snapshot::~snapshot(
) noexcept {
	Winapi::Handle::Close(_handle);
}

bool process::snapshot::find(
	_in _out std::vector<findinfo> &findinfo_s
) {
	auto count = findinfo_s.size();								// число элементов, которые осталось найти

	for (auto &findinfo : findinfo_s) {
		assert(findinfo.profile.details.process_image_filename);
		findinfo.id = 0;
	}
	
	typedef Winapi::Toolhelp32::Process::Entry entry_t;
	entry_t entry;

	auto check = [&findinfo_s, &count](
		_in const entry_t &entry
	) -> bool {
		for (auto &findinfo : findinfo_s) {
			if (0 == entry.th32ProcessID)
				continue;
			if (0 != _wcsicmp(findinfo.profile.details.process_image_filename, entry.szExeFile))
				continue;
			findinfo.id = entry.th32ProcessID;
			assert(count);
			--count;
			return true;
		}
		return false;
	};

	if (!Winapi::Toolhelp32::Process::First(_handle, entry))
		return ERROR_NO_MORE_FILES == Winapi::LastError::Get();
	if (check(entry) && (0 == count))
		return true;

	while (Winapi::Toolhelp32::Process::Next(_handle, entry)) {
		if (check(entry) && (0 == count))
			return true;
	}
	return ERROR_NO_MORE_FILES == Winapi::LastError::Get();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
application::application(
	_in const info &info
) :
	_info(info)
{
	assert(0 != _info.process_id);
}
application::~application(
) noexcept {
}

bool application::check(
) const {
	const console::screen_buffer &screen_buffer = console(_info.process_id).read();
	switch (_info.profile_type) {
		case config::profile::xmrig:
			return check_xmrig(screen_buffer);
		default:
			throw exception();
	}
	return true;
}

/*static*/ bool application::check_xmrig(
	_in const console::screen_buffer &screen_buffer
) {
	auto it_find = std::find(screen_buffer.crbegin(), screen_buffer.crend(), L']');
	if (screen_buffer.crend() == it_find)
		return true;

	struct pattern {
		cstr_t data;
		unsigned size;
	};
#define define_pattern(s)	{s, _countof(s) - 1}
	const pattern pattern_s[] {
		define_pattern(L"DNS error: \"unknown node or service\""),
		define_pattern(L"connect error: \"network is unreachable\""),
		
		//define_pattern(L"speed 10s/60s/15m n/a"),						// 10s
		define_pattern(L"speed 10s/60s/15m n/a n/a"),					// 60s
		//define_pattern(L"speed 10s/60s/15m n/a n/a n/a"),				// 15m, mini
		//define_pattern(L"speed 10s/60s/15m n/a n/a n/a H/s max"),		// 15m, maxi
	};
#undef define_pattern

	std::advance(it_find, -2);
	const auto data = &*it_find;

	for (const auto &pattern : pattern_s)
		if (0 == _wcsnicmp(data, pattern.data, pattern.size)) {
			// trace(L"pattern: \"%s\"", pattern);
			return false;
		}

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
application::console::screen_buffer::screen_buffer(
	_in const rect_type &rect
) :
	_rect(rect),
	std::vector<char_t>(rect.x * rect.y)
{}

const application::console::screen_buffer::rect_type& application::console::screen_buffer::rect(
) const noexcept {
	return _rect;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
application::console::console(
	_in process::id pid
) :
	_handle(attach(pid))
{}
application::console::~console(
) noexcept {
	detach_safe();
}

/*static*/ application::console::handle application::console::attach_safe(
	_in process::id pid
) noexcept {
	if (!Winapi::Console::Attach(pid))
		return nullptr;
	const auto handle = Winapi::Console::Handle::Get(Winapi::Console::Handle::Output);
	if (INVALID_HANDLE_VALUE == handle)
		return nullptr;
	return handle;
}
/*static*/ application::console::handle application::console::attach(
	_in process::id pid
) {
	const auto handle = attach_safe(pid);
	if (handle)
		return handle;
	throw exception();
}

/*static*/ bool application::console::detach_safe(
) noexcept {
	return Winapi::Console::Free();
}
/*static*/ void application::console::detach(
) {
	if (!detach_safe())
		throw exception();
}

bool application::console::get__screen_buffer_info__safe(
	_out screen_buffer::info_type &screen_buffer_info
) const noexcept {
	return Winapi::Console::Output::ScreenBufferInfo::Get(_handle, screen_buffer_info);
}
application::console::screen_buffer::info_type application::console::get__screen_buffer_info(
) const {
	screen_buffer::info_type screen_buffer_info;
	if (get__screen_buffer_info__safe(screen_buffer_info))
		return screen_buffer_info;
	throw exception();
}

bool application::console::read__safe(
	_out screen_buffer &result
) const noexcept {
	result.clear();

	screen_buffer::info_type screen_buffer__info;
	if (!get__screen_buffer_info__safe(screen_buffer__info))
		return false;

	const screen_buffer::rect_type screen_buffer__rect{
		static_cast<decltype(screen_buffer::rect_type::x)>(screen_buffer__info.dwSize.X),
		static_cast<decltype(screen_buffer::rect_type::y)>(screen_buffer__info.srWindow.Bottom - screen_buffer__info.srWindow.Top + 1)
	};
	screen_buffer screen_buffer(screen_buffer__rect);
	DWORD NumberOfCharsRead = 0;
	if (!Winapi::Console::Output::ReadCharacter(
		_handle, screen_buffer.data(), static_cast<DWORD>(screen_buffer.size()), 
		{0, std::max<decltype(COORD::Y)>(0, screen_buffer__info.dwCursorPosition.Y - screen_buffer__rect.y)}, 
		&NumberOfCharsRead
	))
		return false;
	//assert(NumberOfCharsRead == screen_buffer.size());

	result.swap(screen_buffer);
	return true;
}

void application::console::read(
	_out screen_buffer &screen_buffer
) const {
	if (read__safe(screen_buffer))
		return;
	throw exception();
}
application::console::screen_buffer application::console::read(
) const {
	screen_buffer screen_buffer;
	if (read__safe(screen_buffer))
		return screen_buffer;
	throw exception();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
