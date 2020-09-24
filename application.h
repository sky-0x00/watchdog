#pragma once

#include <variant>
#include <optional>
#include "utils\types.h"
#include "config.h"
#include "system.h"

class application {

protected:	
	struct mode {
		// public:
		enum /*class*/ value {
			starter = 0,		// optional
			attacher
		};
		struct args {
			struct starter {
			};
			struct attacher {
				config::profile::type profile_type;
				process::id process_id;
			};
		};
		// public:
		mode(_in value value = value::starter) noexcept;		
		// private:
		value m_value;
		std::variant<args::starter, args::attacher> m_args;
	};

	class args {
	public:
		args(_in argc_t argc, _in argv_t argv[]) noexcept;
	public:
		_set_lasterror(bool) parse__s(_out mode &mode) const noexcept;
		mode parse() const;
	private:
		const argc_t m_argc;
		const argv_t *m_argv;
	};

	class process_attacher {
	public:
		_set_lasterror(process::handle) start(_in const mode::args::attacher &args);
		operator process::handle() const noexcept;
	protected:
		static _set_lasterror(process::handle) s__start(_in const mode::args::attacher &args);
	private:
		handle _handle;
	};

public:
	int run(_in argc_t argc, _in argv_t argv[]) const;
	const string_t& path() const;
protected:
	int runas_starter(_in const mode::args::starter &args) const;
	int runas_attacher(_in const mode::args::attacher &args) const;
protected:
	static string_t get_path();
	static _set_lasterror(cstr_t) get_path__s(_out string_t &path) noexcept;

private:
	mutable std::optional<string_t> _path;

};	// class application
