#include <windows.h>
#include "types.h"

exception::exception(
	_in code code
) : 
	_code(code)
{}
exception::exception(
) :
	exception(/*Winapi*/::GetLastError())
{}

exception::code exception::get_code(
) const noexcept {
	return _code;
}