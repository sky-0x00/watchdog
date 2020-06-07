#include <cassert>
#include "application.h"

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
	auto count = findinfo_s.size();					// осталось заполнить

	for (auto &findinfo : findinfo_s) {
		assert(findinfo.image_filename);
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
			if (0 != _wcsicmp(findinfo.image_filename, entry.szExeFile))
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
	_in pid pid
) :
	_pid(pid)
{
	assert(_pid);
}
application::~application(
) noexcept {
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
	_out screen_buffer_info &screen_buffer_info
) const noexcept {
	return Winapi::Console::Output::ScreenBufferInfo::Get(_handle, screen_buffer_info);
}
application::console::screen_buffer_info application::console::get__screen_buffer_info(
) const {
	screen_buffer_info screen_buffer_info;
	if (get__screen_buffer_info__safe(screen_buffer_info))
		return screen_buffer_info;
	throw exception();
}

bool application::console::read__safe(
) const noexcept {
	return true;
}
void application::console::read(
) const {
	if (read__safe())
		return;
	throw exception();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
