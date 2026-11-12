#include <stdint.h>

const uint16_t BITMAP_MAGIC_NUMBER = 0x4d42;

typedef struct {
	// 参考: https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
	uint16_t type;			// 0x4d42 (ASCII: BM)
	uint32_t size;			// ビットマップファイルのサイズ(byte単位)
	uint16_t reserved_1;	// 0
	uint16_t reserved_2;	// 0
	uint32_t off_bits;		// この構造体の先頭からビットマップビットまでのオフセット(byte単位)
} BITMAP_FILE_HEADER;
