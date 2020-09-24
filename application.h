#pragma once

#include <variant>
#include <optional>
#include "utils\types.h"
#include "config.h"
#include "system.h"

class application {

protected:	
	struct work_mode {
		// public:
		enum /*class*/ value {
			starter = 0,		// optional
			attacher
		};
		template <value> struct details; 
		template <> struct details<starter> {
		};
		template <> struct details<attacher> {
			config::profile::type profile_type;
			process::id process_id;
			details(
				_in config::profile::type profile_type = static_cast<config::profile::type>(0), _in process::id process_id = 0
			) :
				profile_type(profile_type), process_id(process_id)
			{}
		};
		// public:
		work_mode(_in value value = value::starter) noexcept;		
		// private:
		value m_value;
		std::variant<details<starter>, details<attacher>> m_details;
	};

	class details {
	public:
		details(_in argc_t argc, _in argv_t argv[]) noexcept;
	public:
		_set_lasterror(bool) parse__s(_out work_mode &work_mode) const noexcept;
		work_mode parse() const;
	private:
		const argc_t m_argc;
		const argv_t *m_argv;
	};

	class process_attacher {
	public:
		_set_lasterror(bool) start(_in const work_mode::details<work_mode::attacher> &details);
	public:
		const process::information& information() const noexcept;
		operator process::handle() const noexcept;
	protected:
		static _set_lasterror(bool) s__start(_in const work_mode::details<work_mode::attacher> &details, _out process::information &process_information);
	private:
		process::information m_information;
	};

public:
	int run(_in argc_t argc, _in argv_t argv[]) const;
	const string_t& path() const;
protected:
	int runas_starter(_in const work_mode::details<work_mode::starter> &details) const;
	int runas_attacher(_in const work_mode::details<work_mode::attacher> &details) const;
protected:
	static string_t get_path();
	static _set_lasterror(cstr_t) get_path__s(_out string_t &path) noexcept;

private:
	mutable std::optional<string_t> m_path;

};	// class application
