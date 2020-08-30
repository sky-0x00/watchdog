#pragma once

#include <vector>
#include <list>
#include "utils\winapi.h"
#include "config.h"


class application {
public:
	typedef config::profile::map_e<config::profile::details_traits::information>::value_type information;

	class console {
	public:
		typedef handle_t handle;
		class screen_buffer: public std::vector<char_t> {
		public:
			typedef Winapi::Console::Output::ScreenBufferInfo info_type;
			struct rect_type {
				ushort_t x, y;
			};
		public:
			screen_buffer(_in const rect_type &rect = {0, 0});
			const rect_type& rect() const noexcept;
			//const rect_type& rect(_in const rect_type &rect) noexcept;
		private:
			const rect_type _rect;
		};
		
	public:
		console(_in pid_t pid);
		~console();

		bool read__safe(_out screen_buffer &screen_buffer) const noexcept;
		void read(_out screen_buffer &screen_buffer) const;
		screen_buffer read() const;

	protected:
		static handle attach_safe(_in pid_t pid) noexcept;
		static handle attach(_in pid_t pid);
		static bool detach_safe() noexcept;
		static void detach();

		bool get__screen_buffer_info__safe(_out screen_buffer::info_type &screen_buffer_info) const noexcept;
		screen_buffer::info_type get__screen_buffer_info() const;

	private:
		handle _handle;
	};

public:
	application(_in const information &information);
	~application() noexcept;

public:
	bool check() const;			// true: ok, false: need reboot
protected:
	static bool check_xmrig(_in const console::screen_buffer &screen_buffer);

private:
	const information _information;

};	// class app
