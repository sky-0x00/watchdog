//#include <map>
#include <windows.h>
#include "config.h"
#include "console.h"
#include "application.h"
#include "system.h"

int wmain(
	_in unsigned /*argc*/, _in cstr_t /*argv*/[]
) {
	const console console;
	console.echo(L"[%s] configuration: %u profile(s)\n", time().c_str(), config::profile::map.size());
	
	console.echo(L"[%s] system-process searching...", time().c_str());
	const auto process_s = std::move(process::snapshot().find(config::profile::map));
	auto count = process_s.size();
	console.echo(L" ok, %u process(es)\n", count);
	if (0 == count) {
		console.echo(L"[%s] no work, exiting...", time().c_str());
		return Winapi::Status::Success;
	}

	console.echo(L"[%s] system-process dispatching...\n", time().c_str());
	auto is_reboot_needed = false;
	for (const auto &process : process_s) {
		//...
	}
	console.echo(L"[%s] system-process dispatching... ok, is-reboot-needed: %s\n", time().c_str(), is_reboot_needed ? L"yes" : L"no");
	
	if (!is_reboot_needed) {
		console.echo(L"[%s] no work, exiting...", time().c_str());
		return Winapi::Status::Success;
	}
#ifdef _DEBUG
	count = 0;		// сразу, без ожидания
#else
	count = 30;		// ожидаем 30 сек
#endif
	console.echo(L"[%s] system rebooting, %u sec(s)...", time().c_str(), count);
	system::reboot();
	return Winapi::Status::Success;




	//const auto &findinfo = findinfo_s.at(0);
	//console.echo(L"[%s] dispatching process \"%s\"...", time().c_str(), findinfo.profile.details.process_image_filename);
	//const auto is_ok = application({ findinfo.process_id, findinfo.profile.type }).check();
	//wprintf(L"WTF");
	//if (is_ok) {
	//	console.echo(L" ok, check(true)\n[%s] no work, exiting...", time().c_str());
	//	return Winapi::Status::Success;
	//}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
