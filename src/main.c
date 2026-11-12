#include <stdio.h>

#include <windows.h>

#include "bitmap.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {

	FILE* file = fopen("sample.bmp", "rb");

	if (!file) {
		return 1;
	}

	BITMAP_FILE_HEADER bitmap_file_header = { 0 };

	fread(&bitmap_file_header, sizeof(BITMAP_FILE_HEADER), 1, file);
	if (bitmap_file_header.type != BITMAP_MAGIC_NUMBER) {
		MessageBoxW(NULL, L"This is not bitmap", L"Format error", MB_ICONERROR | MB_OK);
		fclose(file);
		return 1;
	}

	MessageBoxW(NULL, L"Success loaded bitmap", L"Success", MB_ICONINFORMATION | MB_OK);

	fclose(file);

	return 0;
}
