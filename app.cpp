#include <cassert>
#include "app.h"
#include "utils\winapi.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
process::snapshot::snapshot(
) :
	_handle(Winapi::Toolhelp32::CreateSnapshot(TH32CS_SNAPPROCESS))
{
	assert(_handle);
}
process::snapshot::~snapshot(
) {
	Winapi::Handle::Close(_handle);
}

bool process::snapshot::find(
	_in _out std::vector<findinfo> &findinfo_s
) {
	auto count = findinfo_s.size();					// осталось заполнить

	for (auto &findinfo : findinfo_s) {
		assert(findinfo.image_filename);
		findinfo.id = 0;
	}
	
	typedef Winapi::Toolhelp32::Process::Entry entry_t;
	entry_t entry;

	auto check = [&findinfo_s, &count](
		_in const entry_t &entry
	) -> bool {
		for (auto &findinfo : findinfo_s) {
			if (0 == entry.th32ProcessID)
				continue;
			if (0 != _wcsicmp(findinfo.image_filename, entry.szExeFile))
				continue;
			findinfo.id = entry.th32ProcessID;
			assert(count);
			--count;
			return true;
		}
		return false;
	};

	if (!Winapi::Toolhelp32::Process::First(_handle, entry))
		return ERROR_NO_MORE_FILES == Winapi::LastError::Get();
	if (check(entry) && (0 == count))
		return true;

	while (Winapi::Toolhelp32::Process::Next(_handle, entry)) {
		if (check(entry) && (0 == count))
			return true;
	}
	return ERROR_NO_MORE_FILES == Winapi::LastError::Get();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
app::app(
	_in process::id pid
) :
	_handle(Winapi::Process::Open(pid, PROCESS_ALL_ACCESS))
{
	assert(_handle);
}
app::~app(
) {
	Winapi::Handle::Close(_handle);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
