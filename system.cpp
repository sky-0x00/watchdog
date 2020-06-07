#include <memory>
#include "system.h"
#include "utils\winapi.h"

/*static*/ bool system::privilege::adjust(
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