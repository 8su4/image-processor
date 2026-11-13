#include <stdint.h>

// 参考: https://learn.microsoft.com/ja-jp/windows/win32/gdi/bitmap-storage
// 　　  https://ja.wikipedia.org/wiki/Windows_bitmap

const uint16_t BITMAP_MAGIC_NUMBER = 0x4d42;

#pragma pack(push, 2)		// アラインメント処理を2バイト単位に変更
typedef struct {
	// 参考: https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
	uint16_t type;			// 0x4d42 (ASCII: BM)
	uint32_t size;			// ビットマップファイルのサイズ(byte単位)
	uint16_t reserved_1;	// 常に0
	uint16_t reserved_2;	// 常に0
	uint32_t off_bits;		// この構造体の先頭からビットマップビットまでのオフセット(byte単位)
} BITMAP_FILE_HEADER;

typedef struct {
	// 参考: https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
	uint32_t size;		// ビットマップヘッダーサイズ
	uint32_t width;		// ビットマップの幅 (px単位)
	uint32_t height;	// ビットマップの高さ (px単位) 正の値はボトムアップ、負の値はトップダウン
	uint16_t planes;	// 常に1
	uint16_t bit_count;	// ピクセル当たりのドット数。取りうる値は 1, 4, 8, 24
	uint32_t compression;	// 圧縮形式
	uint32_t size_image;	// 画像サイズ (byte単位) 非圧縮RGBビットマップの場合は0に設定可能
	uint32_t x_pels_per_meter;	// 水平方向の解像度 (px/m単位)
	uint32_t y_pels_per_meter;	// 垂直方向の解像度 (px/m単位)
	uint32_t clr_used;			// カラーインデックスの数
	uint32_t clr_important;		// 表示するために重要とみなされるカラーインデックスの数
} BITMAP_INFO_HEADER;
#pragma pack(pop)	// アラインメント処理をデフォルトに戻す
