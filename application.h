#pragma once

#include <vector>
#include "utils\winapi.h"

struct process {

	typedef unsigned id;
	typedef handle_t handle;

	struct image {
		typedef cstr_t filename;
	};

	class snapshot {
	public:
		snapshot();
		~snapshot() noexcept;
	public:
		struct findinfo {
			image::filename image_filename;
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
	typedef process::id pid;
	class console {
	public:
		typedef handle_t handle;
		typedef Winapi::Console::Output::ScreenBufferInfo screen_buffer_info;
	public:
		console(_in process::id pid);
		~console();

		bool read__safe() const noexcept;
		void read() const;

	protected:
		static handle attach_safe(_in process::id pid) noexcept;
		static handle attach(_in process::id pid);
		static bool detach_safe() noexcept;
		static void detach();

		bool get__screen_buffer_info__safe(_out screen_buffer_info &screen_buffer_info) const noexcept;
		screen_buffer_info get__screen_buffer_info() const;

	private:
		handle _handle;
	};

public:
	application(_in pid pid);
	~application() noexcept;
protected:
private:
	const pid _pid;

};	// class app
