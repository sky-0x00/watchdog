#include <cassert>
#include "winapi.h"
#include "types.h"

//----------------------------------------------------------------------------------------------------------------
exception::exception(
	_in code code
) : 
	_code(code)
{}
exception::exception(
) :
	exception(Winapi::LastError::Get())
{}

exception::code exception::get_code(
) const noexcept {
	return _code;
}

//----------------------------------------------------------------------------------------------------------------
/*static*/ uint_t string::native::convert::to_uint(
	_in cstr_t str
) {
	assert(str);
	
	str_t pstr = nullptr;
	const auto result = wcstoul(str, &pstr, 10);
	assert(!pstr);

	return result;
}

//----------------------------------------------------------------------------------------------------------------
