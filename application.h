#pragma once

#include <vector>
#include <list>
#include "utils\winapi.h"
#include "config.h"

struct process {

	typedef pid_t id;
	typedef handle_t handle;

	class snapshot {
	public:
		snapshot();
		~snapshot() noexcept;
	public:
		struct findinfo {
			config::profile profile;
			id id;
		};
		bool find(_in /*image_filename*/ _out /*id*/ std::vector<findinfo> &findinfo_s);
	private:
		typedef handle_t handle;
		const handle _handle;
	};

};	// struct process


class application {
public:
	struct info {
		process::id process_id;
		enum config::profile::type profile_type;
	};

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
		console(_in process::id pid);
		~console();

		bool read__safe(_out screen_buffer &screen_buffer) const noexcept;
		void read(_out screen_buffer &screen_buffer) const;
		screen_buffer read() const;

	protected:
		static handle attach_safe(_in process::id pid) noexcept;
		static handle attach(_in process::id pid);
		static bool detach_safe() noexcept;
		static void detach();

		bool get__screen_buffer_info__safe(_out screen_buffer::info_type &screen_buffer_info) const noexcept;
		screen_buffer::info_type get__screen_buffer_info() const;

	private:
		handle _handle;
	};

public:
	application(_in const info &info);
	~application() noexcept;

public:
	bool check() const;			// true: ok, false: need reboot
protected:
	static bool check_xmrig(_in const console::screen_buffer &screen_buffer);

private:
	const info _info;

};	// class app
