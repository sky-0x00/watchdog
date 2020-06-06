#pragma once

#include <vector>
#include "utils\types.h"

struct process {
	typedef unsigned id;
	typedef handle_t handle;

	struct image {
		typedef cstr_t filename;
	};
	class snapshot {
	public:
		snapshot();
		~snapshot();
	public:
		struct findinfo {
			image::filename image_filename;
			id id;
		};
		bool find(_in _out std::vector<findinfo> &findinfo_s);
	private:
		typedef handle_t handle;
		const handle _handle;
	};
};

class app {
public:

public:
	app(_in process::id pid);
	~app();
protected:

private:
	const process::handle _handle;
};
