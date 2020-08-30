#include <cassert>
#include "system.h"
#include "utils\winapi.h"

/*protected static*/ bool system::privilege::adjust(
) {
	const auto tHandle = Winapi::Process::OpenToken(TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY);
	if (!tHandle)
		return false;
	
	// auto-close: tHandle
	std::unique_ptr<std::remove_pointer<decltype(tHandle)>::type, decltype(&Winapi::Handle::Close)> Token(tHandle, &Winapi::Handle::Close);

	TOKEN_PRIVILEGES tPrivileges {1};
	tPrivileges.Privileges[0] = { Winapi::TokenPrivilege().LookupValue(SE_SHUTDOWN_NAME), SE_PRIVILEGE_ENABLED };
	if (!Winapi::TokenPrivilege::Adjust__DAP_F(tHandle, tPrivileges))
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

//---------------------------------------------------------------------------------------------------------------------------------------------
