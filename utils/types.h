#pragma once

#define _in
#define _out
#define _optional

#define _set_lasterror(x) x

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

typedef unsigned argc_t;
typedef cstr_t argv_t;


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
typedef std::wstring string_t;

struct string {
	enum case_sensitivity: bool {
		no = false, yes
	};
	//struct native {
	//};
	template <case_sensitivity case_sensitivity = case_sensitivity::yes> class compare {
	public:
		int operator()(
			_in cstr_t lhs, _in cstr_t rhs
		) const {
			if constexpr (case_sensitivity)
				return ::wcscmp(lhs, rhs);
			else
				return ::wcsicmp(lhs, rhs);
			//return constexpr(case_sensitivity) ? wcscmp(lhs, rhs) : wcsicmp(lhs, rhs);
		}
	public:
		bool is_equal(
			_in cstr_t lhs, _in cstr_t rhs
		) const {
			return 0 == operator()(lhs, rhs);
		}
		//bool is_less...
	};

	struct native {
		struct convert {
			static uint_t to_uint(_in cstr_t str);
		};
	};
};

//#include <vector>
//#include <list>
//struct string : std::wstring {
//	//__if_exists(std::vector) {
//	//	typedef std::vector<string> vector;
//	//}
//	//__if_exists(std::list) {
//	//	typedef std::list<string> list;
//	//}
//};

