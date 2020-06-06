#pragma once

#include <vector>
#include "types.h"

struct process {
	typedef unsigned id;
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
	static process::id find_by__image_filename(_in image::filename image_filename);
};

class app {
public:
	app(_in process::image::filename image_filename);
protected:

private:
	const process::id _pid;
};
