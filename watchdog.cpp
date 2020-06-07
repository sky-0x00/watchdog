#include <windows.h>
#include "config.h"
#include "console.h"
#include "application.h"

int main(
) {
	//console console;
	//console.echo(L"i: %i...", -42);
	//console.color_set({console::color::green}, true);
	//console.echo(L" ok\n");

	//WCHAR Buffer[512];
	//DWORD BytesReaded = 0;

	//auto hStdOut = Winapi::StdHandle::Get(STD_OUTPUT_HANDLE);
	//auto hFile = ::CreateFileW(L"CONOUT$", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	//auto is_ok = FALSE != ::ReadFile(hFile, Buffer, _countof(Buffer), &BytesReaded, nullptr);
	
	auto is_ok = Winapi::Console::Free();

	std::vector<process::snapshot::findinfo> findinfo_s(_countof(config::profile_s));
	{
		auto config_profile = config::profile_s;
		for (auto &findinfo : findinfo_s) {
			findinfo.image_filename = config_profile->details.process_image_filename;
			++config_profile;
		}
	}
	is_ok = process::snapshot().find(findinfo_s);

	string::list string_s;
	application(findinfo_s.at(0).id).read(string_s);

	//is_ok = Winapi::Console::Attach(findinfo_s.at(0).id);
	//auto hStdOutNew = Winapi::StdHandle::Get(STD_OUTPUT_HANDLE);
	//auto hFile = ::CreateFileW(L"CONOUT$", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	//is_ok = FALSE != ::ReadFile(hFile, Buffer, _countof(Buffer), &BytesReaded, nullptr);

	//BytesReaded = 15;
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
