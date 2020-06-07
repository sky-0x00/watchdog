#pragma once

#define _in
#define _out

typedef unsigned char byte_t;

typedef void void_t;
typedef void_t *pvoid_t;
typedef const void_t *cpvoid_t;

typedef wchar_t char_t;
typedef char_t *str_t;
typedef const char_t *cstr_t;

typedef short short_t;
typedef unsigned short ushort_t;

typedef int int_t;
typedef unsigned int uint_t;

typedef void *handle_t;					// handle
typedef unsigned pid_t;					// process-id

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

#include <string>
//typedef std::wstring string;

//#include <vector>
//#include <list>
struct string : std::wstring {
	//__if_exists(std::vector) {
	//	typedef std::vector<string> vector;
	//}
	//__if_exists(std::list) {
	//	typedef std::list<string> list;
	//}
};
