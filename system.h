#pragma once

#include <windows.h>		// SYSTEMTIME(minwinbase.h)
#include <vector>
#include "config.h"

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

	typedef pid_t process_id;
	typedef handle_t handle;

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

};	// struct process

