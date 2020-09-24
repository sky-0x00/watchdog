#include <cassert>

#include "application.h"

#include "utils\winapi.h"
#include "system.h"
#include "console.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
//bool application::check(
//) const {
//	const console::screen_buffer &screen_buffer = console(_information.second.id).read();
//	switch (_information.first) {
//		case config::profile::xmrig:
//			return check_xmrig(screen_buffer);
//		default:
//			throw exception();
//	}
//	return true;
//}
//
//*static*/ bool application::check_xmrig(
//	_in const console::screen_buffer &screen_buffer
//) {
//	auto it_find = std::find(screen_buffer.crbegin(), screen_buffer.crend(), L']');
//	if (screen_buffer.crend() == it_find)
//		return true;
//
//	struct pattern {
//		cstr_t data;
//		unsigned size;
//	};
//#define define_pattern(s)	{s, _countof(s) - 1}
//	const pattern pattern_s[] {
//		define_pattern(L"DNS error: \"unknown node or service\""),
//		define_pattern(L"connect error: \"network is unreachable\""),
//		
//		//define_pattern(L"speed 10s/60s/15m n/a"),						// 10s
//		define_pattern(L"speed 10s/60s/15m n/a n/a"),					// 60s
//		//define_pattern(L"speed 10s/60s/15m n/a n/a n/a"),				// 15m, mini
//		//define_pattern(L"speed 10s/60s/15m n/a n/a n/a H/s max"),		// 15m, maxi
//	};
//#undef define_pattern
//
//	std::advance(it_find, -2);
//	const auto data = &*it_find;
//
//	for (const auto &pattern : pattern_s)
//		if (0 == _wcsnicmp(data, pattern.data, pattern.size)) {
//			// trace(L"pattern: \"%s\"", pattern);
//			return false;
//		}
//
//	return true;
//}
//---------------------------------------------------------------------------------------------------------------------------------------------
//application::console::screen_buffer::screen_buffer(
//	_in const rect_type &rect
//) :
//	_rect(rect),
//	std::vector<char_t>(rect.x * rect.y)
//{}
//
//const application::console::screen_buffer::rect_type& application::console::screen_buffer::rect(
//) const noexcept {
//	return _rect;
//}
//
////---------------------------------------------------------------------------------------------------------------------------------------------
//application::console::console(
//	_in pid_t pid
//) :
//	_handle(nullptr)
//{
//	detach();
//	_handle = attach(pid);
//}
//application::console::~console(
//) noexcept {
//	detach_safe();
//	attach_safe(Winapi::Process::Current::GetId());
//}
//
//*static*/ application::console::handle application::console::attach_safe(
//	_in pid_t pid
//) noexcept {
//	if (!Winapi::Console::Attach(pid))
//		return nullptr;
//	const auto handle = Winapi::Console::Handle::Get(Winapi::Console::Handle::Output);
//	if (INVALID_HANDLE_VALUE == handle)
//		return nullptr;
//	return handle;
//}
//*static*/ application::console::handle application::console::attach(
//	_in pid_t pid
//) {
//	const auto handle = attach_safe(pid);
//	if (handle)
//		return handle;
//	throw exception();
//}
//
//*static*/ bool application::console::detach_safe(
//) noexcept {
//	return Winapi::Console::Free();
//}
//*static*/ void application::console::detach(
//) {
//	if (!detach_safe())
//		throw exception();
//}
//
//bool application::console::get__screen_buffer_info__safe(
//	_out screen_buffer::info_type &screen_buffer_info
//) const noexcept {
//	return Winapi::Console::Output::ScreenBufferInfo::Get(_handle, screen_buffer_info);
//}
//application::console::screen_buffer::info_type application::console::get__screen_buffer_info(
//) const {
//	screen_buffer::info_type screen_buffer_info;
//	if (get__screen_buffer_info__safe(screen_buffer_info))
//		return screen_buffer_info;
//	throw exception();
//}
//
//bool application::console::read__safe(
//	_out screen_buffer &result
//) const noexcept {
//	result.clear();
//
//	screen_buffer::info_type screen_buffer__info;
//	if (!get__screen_buffer_info__safe(screen_buffer__info))
//		return false;
//
//	const screen_buffer::rect_type screen_buffer__rect{
//		static_cast<decltype(screen_buffer::rect_type::x)>(screen_buffer__info.dwSize.X),
//		static_cast<decltype(screen_buffer::rect_type::y)>(screen_buffer__info.srWindow.Bottom - screen_buffer__info.srWindow.Top + 1)
//	};
//	screen_buffer screen_buffer(screen_buffer__rect);
//	DWORD NumberOfCharsRead = 0;
//	if (!Winapi::Console::Output::ReadCharacter(
//		_handle, screen_buffer.data(), static_cast<DWORD>(screen_buffer.size()), 
//		{0, std::max<decltype(COORD::Y)>(0, screen_buffer__info.dwCursorPosition.Y - screen_buffer__rect.y)}, 
//		&NumberOfCharsRead
//	))
//		return false;
//	//assert(NumberOfCharsRead == screen_buffer.size());
//
//	result.swap(screen_buffer);
//	return true;
//}
//
//void application::console::read(
//	_out screen_buffer &screen_buffer
//) const {
//	if (read__safe(screen_buffer))
//		return;
//	throw exception();
//}
//application::console::screen_buffer application::console::read(
//) const {
//	screen_buffer screen_buffer;
//	if (read__safe(screen_buffer))
//		return screen_buffer;
//	throw exception();
//}

//---------------------------------------------------------------------------------------------------------------------------------------------
application::mode::mode(
	_in value value
) noexcept :
	m_value(value)
{}

//---------------------------------------------------------------------------------------------------------------------------------------------
application::args::args(
	_in argc_t argc, _in argv_t argv[]
) noexcept :
	m_argc(argc), m_argv(argv)
{
	assert((0 != m_argc) && m_argv && m_argv[0]);
}

_set_lasterror(bool) application::args::parse__s(
	_out mode &mode
) const noexcept {

	// если аргументов нет, то это starter-режим
	if (1 == m_argc) {
		//mode.m_value = mode::starter;
		//mode.m_args.emplace<mode::args::starter>();
		return true;
	}
	// если аргументов 2, то это attacher-режим
	if (3 == m_argc) {
		// проверка - первый аргумент это profile::type, второй - process::id
		try {
			const mode::args::attacher attacher { 
				static_cast<config::profile::type>(string::native::convert::to_uint(m_argv[1])), string::native::convert::to_uint(m_argv[2])
			};
			mode.m_value = mode::attacher;
			mode.m_args = std::move(attacher);
		}
		catch (...) {
			goto exit_false;
		}
		// проверка - на совпадение путей образов текущего процесса и родительского
		// 1 < m_argc
		// auto np = process::image::normilize_path(m_argv[0]);
		return true;
	}	

exit_false:
	Winapi::LastError::Set(ERROR_BAD_ARGUMENTS);
	return false;
}
application::mode application::args::parse(
) const {
	mode mode;
	if (parse__s(mode))
		return mode;
	throw exception();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
int application::run(
	_in argc_t argc, _in argv_t argv[]
) const {
	const auto mode = args(argc, argv).parse();
	switch (mode.m_value) {
		case mode::starter:
			return runas_starter(std::get<mode::starter>(mode.m_args));
		case mode::attacher:
			return runas_attacher(std::get<mode::attacher>(mode.m_args));
	}
	throw exception();		// code is unreachable
}

int application::runas_starter(
	_in const mode::args::starter &args
) const {
	const console::starter console;

	console.echo(L"[%s] config: %u profile(s)\n", time().c_str(), config::profile::map.size());
	
	console.echo(L"[%s] process searching...", time().c_str());
	const auto process_s = std::move(process::snapshot().find(config::profile::map));
	auto count = process_s.size();
	console.echo(L" ok, %u process(es)\n", count);
	
	{	// scope-begin, it
		auto it = std::make_pair(process_s.cbegin(), process_s.cend());
		for (const auto &profile : config::profile::map) {
			console.echo(L"    %u %s: ", static_cast<unsigned>(profile.first), profile.second.process_image_filename);
			if ((it.first == it.second) || (it.first->first != profile.first))
				console.echo(L"not-found\n");
			else {
				console.echo(L"ok, pid: %u\n", it.first->second.process_id);
				++it.first;
			}
		}
	}	// scope-end, it
	
	console.echo(L"[%s] process dispatching...", time().c_str());
	if (0 == count) {
		console.echo(L" no work\n[%s] exiting...", time().c_str());
		return ERROR_SUCCESS;
	}
	
	auto is_reboot_needed = false;

	for (const auto &process : process_s) {
		DWORD ConsoleProcessList[8];
		auto ConsoleProcessCount = ::GetConsoleProcessList(ConsoleProcessList, _countof(ConsoleProcessList));
		auto hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
		//console::free();
		ConsoleProcessCount = ::GetConsoleProcessList(ConsoleProcessList, _countof(ConsoleProcessList));
		//console::attach(ATTACH_PARENT_PROCESS);
		const auto IsOk = FALSE != ::SetStdHandle(STD_OUTPUT_HANDLE, hConsole);
		ConsoleProcessCount = ::GetConsoleProcessList(ConsoleProcessList, _countof(ConsoleProcessList));
		//console::free();
	}
	console.echo(L"[%s] system-process dispatching... ok, is-reboot-needed: %s\n", time().c_str(), is_reboot_needed ? L"yes" : L"no");

	if (!is_reboot_needed) {
		console.echo(L"[%s] no work, exiting...", time().c_str());
		return ERROR_SUCCESS;
	}
#ifdef _DEBUG
	count = 0;		// сразу, без ожидания
#else
	count = 30;		// ожидаем 30 сек
#endif
	console.echo(L"[%s] system rebooting, %u sec(s)...", time().c_str(), count);
	system::reboot();
	return ERROR_SUCCESS;
}

int application::runas_attacher(
	_in const mode::args::attacher &args
) const {
	return 0;
}

//-- application::process_attacher -------------------------------------------------------------------------------------------------------------------------------------------
application::process_attacher::operator process::handle(
) const noexcept {
	return _handle;
}

/*static protected*/ _set_lasterror(process::handle) application::process_attacher::s__start(
	_in const mode::args::attacher &args
) {
	const auto app_path { application().path() };
	const auto cmd_line { string::format(string::buffer(64 + app_path.size()), L"\"%s %u %u\"", app_path.c_str(), 0, 0) };

	return Winapi::Process::Create(...);
}
_set_lasterror(process::handle) application::process_attacher::start(
	_in const mode::args::attacher &args
) {
	assert(!_handle);
	_handle = std::move(s__start(args));
	return _handle;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
const string_t& application::path(
) const {
	if (!_path)
		_path = get_path();
	return *_path;
}

/*static*/ string_t application::get_path(
) {
	string_t path;
	if (get_path__s(path))
		return path;
	throw exception();
}
/*static*/ _set_lasterror(cstr_t) application::get_path__s(
	_out string_t &path
) noexcept {
	str_t wpgmptr = nullptr;
	if (0 != _get_wpgmptr(&wpgmptr))
		return nullptr;
	return filesystem::path::normilize__s(wpgmptr, path);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
