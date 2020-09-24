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
typedef unsigned pid_t, tid_t;			// process-id, thread-id

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

//template <typename type> struct buffer {
//	type *data;
//	unsigned size;
//};
//template <typename type, unsigned size> struct static_buffer {
//	type data[size];
//};


#include <string>
typedef std::wstring string_t;
typedef std::wstring_view stringview_t;

struct string {
	typedef string_t type;
	typedef unsigned size_type;

	enum case_sensitivity: bool {
		no = false, yes
	};
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
	
	//template <size_type size> using array = char_t[size];
	//
	//template <size_type size> static size_type format_va(
	//	_out array<size> &array, _in cstr_t fstr, _in va_list &details
	//) {
	//	assert(fstr);
	//	const auto count = vswprintf_s(array, std::size(array), fstr, details);
	//	assert(count > 0);
	//	return count;
	//}
	//template <size_type size> static size_type format(
	//	_out array<size> &array, _in cstr_t fstr, _in ...
	//) {
	//	va_list details;
	//
	//	va_start(details, fstr);
	//	const auto count = format_va(array, fstr, details);
	//	va_end(details);
	//
	//	return count;
	//}
	//template <size_type size> static string format(
	//	_in cstr_t fstr, _in ...
	//) {
	//	array<size> array;
	//	va_list details;
	//
	//	va_start(details, fstr);
	//	const auto count = format_va(array, fstr, details);
	//	va_end(details);
	//
	//	return { array, count };
	//}

	class buffer {
	public:
		buffer(_in size_type size);
	public:
		constexpr size_type size() const noexcept;
		str_t data() noexcept;
		cstr_t data() const noexcept;
	public:
		static size_type format(_out buffer &buffer, _in cstr_t fstr, _in ...);
		size_type format(_in cstr_t fstr, _in ...);
	public:
		static size_type format_va(_out buffer &buffer, _in cstr_t fstr, _in va_list &details);
		size_type format_va(_in cstr_t fstr, _in va_list &details);
	private:
		std::unique_ptr<char_t> _data;
		const size_type _size;
	};

	static string_t format(_in _out buffer &&buffer, _in cstr_t fstr, _in ...);
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

