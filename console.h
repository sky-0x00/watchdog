#pragma once

//#include <vadefs.h>
//#include <consoleapi2.h>
//#include <set>
//#include "utils\winapi.h"
//
//class console {
//public:
//	struct color {
//		enum /*class*/ value {
//			blue  = FOREGROUND_BLUE,
//			green = FOREGROUND_GREEN,
//			red   = FOREGROUND_RED
//		};
//		//enum /*class*/ intensity : bool {
//		//	low = false,
//		//	high = true
//		//};
//
//		color(_in const std::set<value> &rgb, _in bool is_intensity_high = false);
//		void clear();
//
//		std::set<value> rgb;
//		bool is_intensity_high;
//	};
//	void color_get(_out color &color) const noexcept;
//	void color_set(_in const color &color) const;
//	void color_set(_in const std::set<color::value> &rgb, _in bool is_intensity_high = false) const;
//	void color_reset() const;
//protected:
//	bool color_push__safe(_in const color &color) const noexcept;
//	bool color_push__safe(_in const std::set<color::value> &rgb, _in bool is_intensity_high = false) const noexcept;
//	bool color_reset__safe() const noexcept;
//
//public:
//	console();
//	~console();
//
//private:
//	struct info_type {
//		typedef Winapi::Console::Output::ScreenBufferInfo screen_buffer_info;
//		typedef short_t text_attr;
//	};
//
//	bool get__screen_buffer_info__safe(_out info_type::screen_buffer_info &screen_buffer_info) const noexcept;
//	info_type::screen_buffer_info get__screen_buffer_info() const;
//
//	bool set_info__text_attr__safe(_in const info_type::text_attr &text_attr) const noexcept;
//	void set_info__text_attr(_in const info_type::text_attr &text_attr) const;
//
//private:
//	typedef handle_t handle;
//	const handle _handle;
//
//private:
//	typedef short_t text_attr;
//
//	text_attr get__text_attr() const;
//	bool set__text_attr__safe(_in const text_attr &text_attr) const noexcept;
//	void set__text_attr(_in const text_attr &text_attr) const;
//
//	text_attr _text_attr;
//
///*static*/ public:
//	static _set_lasterror(bool) attach__s(_in pid_t pid = -1/*ATTACH_PARENT_PROCESS*/) noexcept;
//	static void attach(_in pid_t pid = -1/*ATTACH_PARENT_PROCESS*/);
//	static _set_lasterror(bool) allocate__s() noexcept;
//	static void allocate();
//	static _set_lasterror(bool) free__s() noexcept;
//	static void free();
//
//	static _set_lasterror(bool) get__title__s(_out string &title) noexcept;
//	static string get__title();
//	static _set_lasterror(bool) get__original_title__s(_out string &title) noexcept;
//	static string get__original_title();
//
//};	// class console

#include "utils\types.h"

class console {
public:
	typedef handle_t handle;

	class starter;
	class attacher;

	//	class screen_buffer : public std::vector<char_t> {
	//	public:
	//		typedef Winapi::Console::Output::ScreenBufferInfo info_type;
	//		struct rect_type {
	//			ushort_t x, y;
	//		};
	//	public:
	//		screen_buffer(_in const rect_type &rect = { 0, 0 });
	//		const rect_type& rect() const noexcept;
	//		//const rect_type& rect(_in const rect_type &rect) noexcept;
	//	private:
	//		const rect_type _rect;
	//	};
	//public:
	//	console(_in pid_t pid);
	//	~console();
	//
	//	bool read__safe(_out screen_buffer &screen_buffer) const noexcept;
	//	void read(_out screen_buffer &screen_buffer) const;
	//	screen_buffer read() const;
	//
	//protected:
	//	static handle attach_safe(_in pid_t pid) noexcept;
	//	static handle attach(_in pid_t pid);
	//	static bool detach_safe() noexcept;
	//	static void detach();
	//
	//	bool get__screen_buffer_info__safe(_out screen_buffer::info_type &screen_buffer_info) const noexcept;
	//	screen_buffer::info_type get__screen_buffer_info() const;
};

class console::starter {
public:
	//starter() = default;
	void echo(_in cstr_t format, _in ...) const;
protected:
	void echo(_in cstr_t format, _in va_list args) const;
};
