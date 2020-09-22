#include <cassert>
#include "system.h"

/*protected static*/ bool system::privilege::adjust(
) {
	const auto tHandle = Winapi::Process::OpenToken(TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY);
	if (!tHandle)
		return false;
	// auto-close: tHandle
	std::unique_ptr<std::remove_pointer<decltype(tHandle)>::type, decltype(&Winapi::Handle::Close)> Token(tHandle, &Winapi::Handle::Close);

	TOKEN_PRIVILEGES tPrivileges {1};
	tPrivileges.Privileges[0] = { Winapi::TokenPrivilege().LookupValue(SE_SHUTDOWN_NAME), SE_PRIVILEGE_ENABLED };
	if (!Winapi::TokenPrivilege::Adjust(tHandle, tPrivileges))
		return false;

	return true;
}

/*static*/ bool system::reboot(
	_in unsigned timeout_sec /*= 0*/
) {
	if (!privilege::adjust())
		return false;
	return Winapi::SystemShutdown().Initiate(timeout_sec, true, true);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
time::time(
) :
	_value(get())
{}

cstr_t time::c_str(
) {
	return swprintf_s(_buffer, L"%04u-%02u-%02u %02u:%02u:%02u.%03u",
		_value.wYear, _value.wMonth, _value.wDay, _value.wHour, _value.wMinute, _value.wSecond, _value.wMilliseconds
	) > 0 ? _buffer : nullptr;
}

/*static*/ time::value time::get(
) {
	return Winapi::Time::GetLocal();
}

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

void process::snapshot::find(
	_in const find_in &in, _out find_out &out
) const {
	out = std::move(find(in));
}

process::snapshot::find_out process::snapshot::find(
	_in const find_in &in
) const {
	find_out out;

	if (in.empty())
		return out;

	auto count_remain = in.size();		// осталось найти

	typedef Winapi::Toolhelp32::Process::Entry entry_t;
	entry_t entry;

	auto check_entry = [&in](			// поиск элемента снапшота по имени файла
		_in const entry_t &entry
	) -> const find_in::key_type* {
		if (0 == entry.th32ProcessID)
			return nullptr;
		for (const auto &in_entry : in) {
			if (0 != _wcsicmp(in_entry.second.process_image_filename, entry.szExeFile))
				continue;
			return &in_entry.first;
		}
		return nullptr;
	};

	if (!Winapi::Toolhelp32::Process::First(_handle, entry)) {
		if (Winapi::Status::NoMoreFiles == Winapi::LastError::Get())
			return out;
		throw exception();
	}
	auto result = check_entry(entry);
	if (result) {
		out.emplace(*result, config::profile::details<config::profile::details_traits::information>{entry.th32ProcessID});
		if (0 == --count_remain)
			return out;
	}

	while (Winapi::Toolhelp32::Process::Next(_handle, entry)) {
		result = check_entry(entry);
		if (!result)
			continue;
		out.emplace(*result, config::profile::details<config::profile::details_traits::information>{entry.th32ProcessID});
		if (0 == --count_remain)
			return out;
	}
	if (Winapi::Status::NoMoreFiles == Winapi::LastError::Get())
		return out;
	throw exception();
}

//-- process::image -------------------------------------------------------------------------------------------------------------------------------------------
/*static*/ _set_lasterror(cstr_t) process::image::normilize_path__s(
	_in cstr_t path, _out string_t &result
) noexcept {
	auto size = Winapi::File::GetFullPathName(path, nullptr, 0);
	if (0 == size)
		return nullptr;
	std::vector<char_t> buffer(size);
	size = Winapi::File::GetFullPathName(path, buffer.data(), size);
	if (size >= buffer.size())
		return nullptr;
	result.assign(buffer.data(), size);
	return result.c_str();
}
/*static*/ string_t process::image::normilize_path(
	_in cstr_t path
) {
	string_t result;
	if (normilize_path__s(path, result))
		return result;
	throw exception();
}

/*static*/ string_t process::image::get_path(
	_in process::handle h_process /*= Winapi::Process::Current::Get()*/
) {
	return L"";
}
/*static*/ _set_lasterror(cstr_t) process::image::get_path__s(
	_out string_t &path, _in process::handle h_process /*= Winapi::Process::Current::Get()*/
) noexcept {
	return nullptr;
}

//-- process -------------------------------------------------------------------------------------------------------------------------------------------
process::id process::get_parent(
) {
	return 0;	// DUMMY
}

//-- handle -------------------------------------------------------------------------------------------------------------------------------------------
handle::handle(
	_in value value /*= nullptr*/
) noexcept :
	_value(value)
{}
handle::handle(
	_in _out handle &&handle
) noexcept :
	_value(std::exchange(handle._value, nullptr))
{}

handle::~handle(
) noexcept {
	if (_value)
		Winapi::Handle::Close(_value);
}

handle::operator value(
) const noexcept {
	return _value;
}

handle& handle::operator =(
	_in _out handle &&handle
) {
	assert(!_value);
	_value = std::exchange(handle._value, nullptr);
	return *this;
}

_set_lasterror(bool) handle::close(
) noexcept {
	if (!_value) {
		Winapi::LastError::Set(ERROR_INVALID_HANDLE);
		return true;
	}
	if (Winapi::Handle::Close(_value)) {
		Winapi::LastError::Set(ERROR_SUCCESS);
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------------------------
