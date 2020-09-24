#pragma once

#include <windows.h>		// SYSTEMTIME(minwinbase.h)
#include <vector>
#include "config.h"
#include "utils\winapi.h"

class system {
public:
	static bool reboot(_in unsigned timeout_sec = 0);
protected:
	struct privilege {
		static bool adjust();
	};
};	// class system

class time {
public:
	time();
	cstr_t c_str();
protected:
	typedef SYSTEMTIME value;
	static value get();
private:
	const value _value;
	char_t _buffer[32];
};	// class time

struct process {

	typedef pid_t id;
	typedef handle_t handle;

	struct information {
		handle handle;
		id id;
		information(_in process::handle handle = nullptr, _in process::id id = 0);
	};

	struct image {
		// через систему, с получением информации о процессе
		static string_t get_path(_in process::handle h_process = Winapi::Process::Current::Get());
		static _set_lasterror(cstr_t) get_path__s(_out string_t &path, _in process::handle h_process = Winapi::Process::Current::Get()) noexcept;

		// через номализацию пути, переданного в cmd-окружении консоли (перенес в класс application)
		//static string_t get_path();
		//static _set_lasterror(cstr_t) get_path__s(_out string_t &path) noexcept;
	};

	class snapshot {
	public:
		snapshot();
		~snapshot() noexcept;
	public:
		typedef config::profile::map_e<config::profile::details_traits::search> find_in;
		typedef config::profile::map_e<config::profile::details_traits::information> find_out;
		void find(_in const find_in &in, _out find_out &out) const;
		find_out find(_in const find_in &in) const;
	private:
		typedef handle_t handle;
		const handle _handle;
	};

	static id get_parent();

};	// struct process

class handle {
public:
	typedef handle_t value;
public:
	handle(_in value value = nullptr) noexcept;
	handle(_in const handle &handle) = delete;
	handle(_in _out handle &&handle) noexcept;
	handle& operator =(_in const handle &handle) = delete;
	handle& operator =(_in _out handle &&handle);
	~handle() noexcept;
	operator value() const noexcept;
	_set_lasterror(bool) close() noexcept;
private:
	value _value;
};

namespace filesystem {
	struct path {
		static string_t normilize(_in cstr_t path);
		static _set_lasterror(cstr_t) normilize__s(_in cstr_t path, _out string_t &result) noexcept;
	};
};