#pragma once

#define _in
#define _out

typedef unsigned char byte_t;

typedef void void_t;
typedef void_t *pvoid_t, *handle_t;
typedef const void_t *cpvoid_t;

typedef wchar_t char_t;
typedef char_t *str_t;
typedef const char_t *cstr_t;

typedef short short_t;
typedef unsigned short ushort_t;


class exception {
public:
	typedef unsigned code;
	exception();
	exception(_in code code);
public:
	code get_code() const noexcept;
private:
	const code _code;
};