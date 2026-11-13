#include <stdio.h>

#include <windows.h>

#include "bitmap.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {

	FILE* file;
	if (!(file = fopen("sample.bmp", "rb"))) {
		return 1;
	}

	BITMAP_FILE_HEADER bitmap_file_header = { 0 };
	fread(&bitmap_file_header, sizeof(BITMAP_FILE_HEADER), 1, file);

	wchar_t buf[256];
	_snwprintf_s(
		buf,
		_countof(buf),
		_TRUNCATE,
		L"Bitmap type: %x\nFile size: %u\nOffset pos: %u\nNow offset: %u\n",
		bitmap_file_header.type,
		bitmap_file_header.size,
		bitmap_file_header.off_bits,
		ftell(file)
	);
	OutputDebugStringW(buf);

	if (bitmap_file_header.type != BITMAP_MAGIC_NUMBER) {
		MessageBoxW(NULL, L"This is not bitmap", L"Format error", MB_ICONERROR | MB_OK);
		fclose(file);
		return 1;
	}

	MessageBoxW(NULL, L"Success loaded bitmap", L"Success", MB_ICONINFORMATION | MB_OK);

	fclose(file);

	return 0;
}
