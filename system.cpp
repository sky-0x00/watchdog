#include "system.h"
#include "utils\winapi.h"

/*static*/ bool system::privileges::adjust(
) {
	return true;
}

/*static*/ bool system::reboot(
	_in unsigned timeout_sec /*= 0*/
) {
	return Winapi::Shutdown().Initiate(timeout_sec, true, true);
}