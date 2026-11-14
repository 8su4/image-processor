#include <stdio.h>

#include <windows.h>

#include "bitmap.h"

#define IDM_FILEOPEN 0

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	wchar_t bitmap_info[1024] = { 0 };

	switch (uMsg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDM_FILEOPEN: {
			wchar_t pwstrFilePath[256] = L"";
			wchar_t pwstrFileName[64] = L"";
			OPENFILENAMEW openFileName = {
				.lStructSize = sizeof(OPENFILENAME),
				.hwndOwner = hWnd,
				.hInstance = NULL,
				.lpstrFilter = L"BITMAP images\0*.bmp\0\0",
				.lpstrFile = pwstrFilePath,
				.nMaxFile = sizeof(pwstrFilePath) / sizeof(pwstrFilePath[0]),
				.lpstrFileTitle = pwstrFileName,
			};

			if (GetOpenFileNameW(&openFileName)) {
				char* pstrFilePath = malloc(256);
				wcstombs_s(NULL, pstrFilePath, 256, pwstrFilePath, 255);
				if (pstrFilePath == NULL) {
					return 0;
				}

				FILE* file = NULL;
				if (fopen_s(&file, pstrFilePath, "rb") != 0) {
					if (pstrFilePath) {
						free(pstrFilePath);
					}
					return 1;
				}
				if (pstrFilePath) {
					free(pstrFilePath);
				}

				BITMAP_FILE_HEADER bitmap_file_header = { 0 };
				fread(&bitmap_file_header, sizeof(BITMAP_FILE_HEADER), 1, file);

				if (bitmap_file_header.type != BITMAP_MAGIC_NUMBER) {
					MessageBoxW(NULL, L"This is not bitmap", L"Format error", MB_ICONERROR | MB_OK);
					fclose(file);
					return 1;
				}

				BITMAP_INFO_HEADER bitmap_info_header = { 0 };
				fread(&bitmap_info_header, sizeof(BITMAP_INFO_HEADER), 1, file);

				_snwprintf_s(
					bitmap_info,
					_countof(bitmap_info),
					_TRUNCATE,
					L"Bitmap type: %x\nFile size: %u\nOffset pos: %u\n  Header size: %u\n  width: %u\n  height: %u\n  bit per px: %u\n",
					bitmap_file_header.type,
					bitmap_file_header.size,
					bitmap_file_header.off_bits,
					bitmap_info_header.size,
					bitmap_info_header.width,
					bitmap_info_header.height,
					bitmap_info_header.bit_count
				);
				OutputDebugStringW(bitmap_info);

				fclose(file);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		}
		return 0;
	}
	case WM_PAINT: {
		RECT rect;
		GetClientRect(hWnd, &rect);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		DrawTextW(hdc, bitmap_info, -1, &rect, 0);
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
};

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {

	WNDCLASSEXW wcex = {
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_VREDRAW | CS_HREDRAW,
		.lpfnWndProc = WndProc,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = hInstance,
		.hIcon = LoadIconW(NULL, IDI_APPLICATION),
		.hCursor = LoadCursorW(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
		.lpszMenuName = NULL,
		.lpszClassName = L"ImageProcessorClass",
		.hIconSm = LoadIconW(NULL, IDI_APPLICATION),
	};

	if (!RegisterClassExW(&wcex)) {
		MessageBoxW(NULL, L"Failed to register windowclass.", L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	HMENU hMenu = CreateMenu();
	HMENU hMenuFile = CreateMenu();
	MENUITEMINFOW menuFileItemInfo = {
		.cbSize = sizeof(MENUITEMINFO),
		.fMask = MIIM_ID | MIIM_STRING,
		.wID = IDM_FILEOPEN,
		.dwTypeData = L"開く(&O)...\tCtrl+O"
	};
	InsertMenuItemW(hMenuFile, 0, MF_BYCOMMAND, &menuFileItemInfo);

	MENUITEMINFOW menuItemInfo = {
		.cbSize = sizeof(MENUITEMINFO),
		.fMask = MIIM_SUBMENU | MIIM_STRING,
		.hSubMenu = hMenuFile,
		.dwTypeData = L"ファイル (&F)"
	};
	InsertMenuItemW(hMenu, 0, MF_BYCOMMAND, &menuItemInfo);

	HWND hWnd = CreateWindowExW(
		0,
		wcex.lpszClassName,
		L"ImageProcessor",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		hMenu,
		hInstance,
		NULL
	);

	if (!hWnd) {
		MessageBoxW(NULL, L"Failed to create window.", L"Error", MB_ICONERROR | MB_OK);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessageW(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}
