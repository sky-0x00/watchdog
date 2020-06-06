#include <windows.h>
#include "config.h"
#include "console.h"
#include "app.h"

int main(
) {
	//console console;
	//console.echo(L"i: %i...", -42);
	//console.color_set({console::color::green}, true);
	//console.echo(L" ok\n");

	auto hStdOut = Winapi::StdHandle::Get(STD_OUTPUT_HANDLE);
	auto is_ok = Winapi::Console::Free();

	std::vector<process::snapshot::findinfo> findinfo_s(_countof(config::known_process_image_filenames));
	{
		auto config__known_process_image_filename = config::known_process_image_filenames;
		for (auto &findinfo : findinfo_s)
			findinfo.image_filename = *config__known_process_image_filename++;
	}
	is_ok = process::snapshot().find(findinfo_s);
	is_ok = Winapi::Console::Attach(findinfo_s.at(0).id);
	auto hStdOutNew = Winapi::StdHandle::Get(STD_OUTPUT_HANDLE);
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
