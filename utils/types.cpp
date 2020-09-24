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
	assert(L'\0' == *pstr);

	return result;
}

//----------------------------------------------------------------------------------------------------------------
string::buffer::buffer(
	_in size_type size
) :
	/*const*/ _size(size), _data(new char_t[size])
{
	assert(_data.get());
}

constexpr string::size_type string::buffer::size(
) const noexcept {
	return _size;
}

str_t string::buffer::data(
) noexcept {
	return _data.get();
}
cstr_t string::buffer::data(
) const noexcept {
	return _data.get();
}

//----------------------------------------------------------------------------------------------------------------
/*static*/ string::size_type string::buffer::format_va(
	_out buffer &buffer, _in cstr_t fstr, _in va_list &details
) {
	assert(fstr);
	const auto count = vswprintf_s(buffer.data(), buffer.size(), fstr, details);
	assert(count > 0);
	return count;
}
string::size_type string::buffer::format_va(
	_in cstr_t fstr, _in va_list &details
) {
	return format_va(*this, fstr, details);
}

/*static*/ string::size_type string::buffer::format(
	_out buffer &buffer, _in cstr_t fstr, _in ...
) {
	va_list details;

	va_start(details, fstr);
	const auto count = format_va(buffer, fstr, details);
	va_end(details);

	return count;
}
string::size_type string::buffer::format(
	_in cstr_t fstr, _in ...
) {
	va_list details;

	va_start(details, fstr);
	const auto count = format_va(fstr, details);
	va_end(details);

	return count;
}

//----------------------------------------------------------------------------------------------------------------
/*static*/ string_t string::format(
	_in _out buffer &&buffer, _in cstr_t fstr, _in ...
) {
	va_list details;

	va_start(details, fstr);
	const auto count = buffer.format_va(fstr, details);
	va_end(details);

	return { buffer.data(), count };
}

//----------------------------------------------------------------------------------------------------------------
