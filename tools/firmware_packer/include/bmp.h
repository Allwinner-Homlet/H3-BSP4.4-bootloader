/*
************************************************************************************************************************
*
* File Name : bmp.h
*
* Author : Gary.Wang
*
* Version : 1.1.0
*
* Date : 2008.06.27
*
* Description : 
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Gary.Wang      2008.06.27       1.1.0        build the file
*
************************************************************************************************************************
*/
#ifndef  __bmp_h
#define  __bmp_h



enum _bmp_color_mode_e
{
	BMP_COLOR_MODE_24BITS,
	BMP_COLOR_MODE_32BITS
};


enum _origin_pos_e
{
	ORIGIN_POS_UPPER_LEFT,
    ORIGIN_POS_LOWER_LEFT
};


#ifdef EPDK_WIN32
	#pragma pack(push, 1)
#endif
typedef __PACKED struct _full_color_t
{
	uint8  B;
	uint8  G;
	uint8  R;
	uint8  A;
}full_color_t;
#ifdef EPDK_WIN32
	#pragma pack(pop)
#endif



#ifdef EPDK_WIN32
	#pragma pack(push, 1)
#endif
typedef __PACKED struct _bmp_file_head_t
{
	char    bfType[2];        // "BM"
	uint32  bfSize;           // total size of bmp file
	uint32  bfReserved;       // reserved filed
	uint32  bfOffBits;        // pixel matrix offset from file head
}bmp_file_head_t;
#ifdef EPDK_WIN32
	#pragma pack(pop)
#endif

#ifdef EPDK_WIN32
	#pragma pack(push, 1)
#endif
typedef __PACKED struct _bmp_info_head_t
{
	uint32  biSize;           // the size of bmp information head
	uint32  biWidth;          // the width of bmp, unit: pixel
	uint32  biHeight;         // the height of bmp, unit:pixel
	uint16  biPlanes;         // always 1
	uint16  biBitCount;       // bits per pixel. 1-mono, 4-16color, 8-256color, 24-truecolor
	uint32  biCompression;    // copression format. 0: no compression.
	uint32  biSizeImage;      // the size of pixel matirx.
	uint32  biXPelsPerMeter;  // horizontal dis, unit : pixel/meter
	uint32  biYPelsPerMeter;  // vertical dis, unit : pixel/meter
	uint32  biClrUsed;        // number of used colors, 0 means 2^biBitCount
	uint32  biClrImportant;   // number of important colors, 0 means that all colors are important.
}bmp_info_head_t;
#ifdef EPDK_WIN32
	#pragma pack(pop)
#endif



#ifdef EPDK_WIN32
	#pragma pack(push, 1)
#endif
typedef __PACKED struct _tc_bmp_file_format_t
{
	bmp_file_head_t  file_head;         // bmp file head;
	bmp_info_head_t  info_head;         // bmp information head
	full_color_t     matrix[1];         // pixel matrix
}tc_bmp_file_format_t;
#ifdef EPDK_WIN32
	#pragma pack(pop)
#endif


typedef struct _HBMP_i_t
{
	G_FILE *fp;
	uint32  color_mode;
	uint32  color_type_size;
	uint16  bitcount;
	uint32  width;
	uint32  height;
	uint32  row_size;
}HBMP_i_t, *HBMP_i;



typedef struct _color_24_t
{
	uint8  B;
	uint8  G;
	uint8  R;
}color_24_t;


typedef uint32 HBMP;


extern HBMP  create_bmp( const char *bmp_file, uint32 width, uint32 height, const full_color_t *color_p, int32 color_mode );
extern HBMP  open_bmp( const char *bmp_file );
extern int32 close_bmp ( HBMP handle );
extern int32 draw_pixel( HBMP handle, uint32 x, uint32 y, const full_color_t *color_p, int32 origin_pos );
extern int32 read_pixel( HBMP handle, uint32 x, uint32 y, full_color_t *color_p, int32 origin_pos );






#endif     //  ifndef __bmp_h

/* end of bmp.h  */
