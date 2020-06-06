#pragma once

#include <vadefs.h>
#include <consoleapi2.h>
#include <set>
#include "utils\winapi.h"

class console {
public:
	struct color {
		enum /*class*/ value {
			blue  = FOREGROUND_BLUE,
			green = FOREGROUND_GREEN,
			red   = FOREGROUND_RED
		};
		//enum /*class*/ intensity : bool {
		//	low = false,
		//	high = true
		//};

		color(_in const std::set<value> &rgb, _in bool is_intensity_high = false);
		void clear();

		std::set<value> rgb;
		bool is_intensity_high;
	};
	void color_get(_out color &color) const noexcept;
	void color_set(_in const color &color) const;
	void color_set(_in const std::set<color::value> &rgb, _in bool is_intensity_high = false) const;
	void color_reset() const;
protected:
	bool color_push__safe(_in const color &color) const noexcept;
	bool color_push__safe(_in const std::set<color::value> &rgb, _in bool is_intensity_high = false) const noexcept;
	bool color_reset__safe() const noexcept;

public:
	console();
	~console();

public:
	void echo(_in cstr_t format, _in ...) const;	
protected:
	void echo(_in cstr_t format, _in va_list args) const;

private:
	struct info {
		typedef Winapi::Console::ScreenBufferInfo screen_buffer;
		typedef short_t text_attr;
	};

	bool get_info__screen_buffer__safe(_out info::screen_buffer &screen_buffer) const noexcept;
	info::screen_buffer get_info__screen_buffer() const;

	bool set_info__text_attr__safe(_in const info::text_attr &text_attr) const noexcept;
	void set_info__text_attr(_in const info::text_attr &text_attr) const;

private:
	struct handle {
		typedef handle_t value;
	};
	const handle::value _handle;

protected:
	typedef short_t text_attr;

	text_attr get__text_attr() const;
	bool set__text_attr__safe(_in const text_attr &text_attr) const noexcept;
	void set__text_attr(_in const text_attr &text_attr) const;

	const text_attr _text_attr;

};	// class console
