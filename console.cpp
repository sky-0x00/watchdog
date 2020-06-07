#include <windows.h>
#include <stdio.h>
#include "console.h"

console::color::color(
	_in const std::set<value> &rgb, _in bool is_intensity_high /*= false*/
) :
	rgb(rgb), is_intensity_high(is_intensity_high)
{}
void console::color::clear(
) {
	rgb.clear();
	is_intensity_high = false;
}


console::console(
) : 
	_handle(Winapi::Console::Handle::Get(Winapi::Console::Handle::Output)),
	_text_attr(get__text_attr())
{}
console::~console(
) {
	color_reset__safe();
}

void console::echo(
	_in cstr_t format, _in va_list args
) const {
	vwprintf_s(format, args);
}
void console::echo(
	_in cstr_t format, _in ...
) const {
	va_list args;
	va_start(args, format);
	echo(format, args);
	va_end(args);
}

bool console::get__screen_buffer_info__safe(
	_out info_type::screen_buffer_info &screen_buffer_info
) const noexcept {
	return Winapi::Console::Output::ScreenBufferInfo::Get(_handle, screen_buffer_info);
}
console::info_type::screen_buffer_info console::get__screen_buffer_info(
) const {
	info_type::screen_buffer_info screen_buffer_info;
	if (get__screen_buffer_info__safe(screen_buffer_info))
		return screen_buffer_info;
	throw exception();
}

bool console::set_info__text_attr__safe(
	_in const info_type::text_attr &text_attr
) const noexcept {
	return Winapi::Console::Output::TextAttribute::Set(_handle, text_attr);
}
void console::set_info__text_attr(
	_in const info_type::text_attr &text_attr
) const {
	if (!set_info__text_attr__safe(text_attr))
		throw exception();
}


console::text_attr console::get__text_attr(
) const {
	return get__screen_buffer_info().wAttributes;
}

bool console::set__text_attr__safe(
	_in const text_attr &text_attr
) const noexcept {
	return set_info__text_attr__safe(text_attr);
}
void console::set__text_attr(
	_in const text_attr &text_attr
) const {
	if (!set__text_attr__safe(text_attr))
		throw exception();
}


bool console::color_push__safe(
	_in const std::set<color::value> &rgb, _in bool is_intensity_high /*= false*/
) const noexcept {
	text_attr text_attr = _text_attr & 0xFFF0;
	for (const auto &rgb : rgb)
		text_attr |= static_cast<decltype(text_attr)>(rgb);
	if (is_intensity_high)
		text_attr |= FOREGROUND_INTENSITY;
	return set__text_attr__safe(text_attr);
}
void console::color_set(
	_in const std::set<color::value> &rgb, _in bool is_intensity_high /*= false*/
) const {
	if (!color_push__safe(rgb, is_intensity_high))
		throw exception();
}

bool console::color_push__safe(
	_in const color &color
) const noexcept {
	return color_push__safe(color.rgb, color.is_intensity_high);
}
void console::color_set(
	_in const color &color
) const {
	if (!color_push__safe(color))
		throw exception();
}

void console::color_get(
	_out color &color
) const noexcept {
	color.clear();

	if (_text_attr & static_cast<decltype(_text_attr)>(color::blue))
		color.rgb.insert(color::blue);
	if (_text_attr & static_cast<decltype(_text_attr)>(color::green))
		color.rgb.insert(color::green);
	if (_text_attr & static_cast<decltype(_text_attr)>(color::red))
		color.rgb.insert(color::red);
	if (_text_attr & FOREGROUND_INTENSITY)
		color.is_intensity_high = true;
}

bool console::color_reset__safe(
) const noexcept {
	return set__text_attr__safe(_text_attr);
}
void console::color_reset(
) const {
	set__text_attr(_text_attr);
	//if (!color_reset__safe())
	//	throw exception();
}