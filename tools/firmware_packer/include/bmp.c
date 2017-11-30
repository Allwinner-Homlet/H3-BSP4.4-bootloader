/*
************************************************************************************************************************
*
* File Name : bmp.c
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
#ifndef  __bmp_c
#define  __bmp_c


#include "bmp.h"



/*-------------------------------------------function list----------------------------------------*/

HBMP  create_bmp( const char *bmp_file, uint32 width, uint32 height, const full_color_t *color_p, int32 color_mode )
{
	G_FILE *fp;
	bmp_file_head_t  file_head;
	bmp_info_head_t  info_head;
	HBMP_i hdl;
	uint32 i;
	uint32 j;
	uint32 k;
	uint8  row_append_content;
	uint32 row_append_count;
	uint32 color_type_size;
	
	if( bmp_file == NULL  ||  color_p == NULL )
		return NULL;
		
	if( height == 0 || width == 0 )
		return NULL;
		
	if( color_mode != BMP_COLOR_MODE_24BITS  &&  color_mode != BMP_COLOR_MODE_32BITS )
		return NULL;
		
	fp = g_fopen( bmp_file, "wb+" );
	if( fp == NULL )
		return NULL;
	
	/* init HBMP */
	hdl = (HBMP_i)g_malloc( sizeof(HBMP_i_t) );
	if( hdl == NULL )
	{
		g_fclose( fp );
		return NULL;
	}
	
	hdl->fp = fp;
	hdl->color_mode = color_mode;
	hdl->width = width;
	hdl->height = height;
	if( color_mode == BMP_COLOR_MODE_24BITS )
		hdl->bitcount = 24;
	else if( color_mode == BMP_COLOR_MODE_32BITS )
		hdl->bitcount = 32;
	else
		goto error;
	color_type_size = hdl->bitcount >> 3;
	hdl->color_type_size = color_type_size;
	hdl->row_size = ( width * color_type_size + 4 - 1 ) & (~( 4 - 1 ) );
	
	/* init file head */
	file_head.bfType[0]   = 'B';
	file_head.bfType[1]   = 'M';
	file_head.bfSize      = sizeof(bmp_file_head_t) + sizeof(bmp_info_head_t) + hdl->row_size * height;
	file_head.bfReserved  = 0;
	file_head.bfOffBits   = sizeof(bmp_file_head_t) + sizeof(bmp_info_head_t);
	g_fwrite( &file_head, 1, sizeof(file_head), fp );
	
	/* init infomation head */
	info_head.biSize          = sizeof(bmp_info_head_t);
	info_head.biWidth         = width;
	info_head.biHeight        = height;
	info_head.biPlanes        = 1;
	info_head.biBitCount      = hdl->bitcount;
	info_head.biCompression   = 0;
	info_head.biSizeImage     = hdl->row_size * height;
	info_head.biXPelsPerMeter = 0xB12;
	info_head.biYPelsPerMeter = 0xB12;
	info_head.biClrUsed       = 0;
	info_head.biClrImportant  = 0;
	g_fwrite( &info_head, 1, sizeof(info_head), fp );
	
	/* init pixel matrix */
	row_append_content = 0;
	row_append_count   = hdl->row_size - width * color_type_size;
	for( i = 0; i < height;  i++ )
	{
		for( j = 0;  j < width;  j++ )
			g_fwrite( color_p, 1, color_type_size, fp );
		for( k = 0;  k < row_append_count;  k++ )
			g_fwrite( &row_append_content, 1, 1, fp );
	}
	
	return (HBMP)hdl;	
	
error:
	g_free( hdl );
	g_fclose( fp );
	return NULL;	
}




HBMP  open_bmp( const char *bmp_file )
{
	G_FILE *fp;
	bmp_file_head_t  file_head;
	bmp_info_head_t  info_head;
	HBMP_i hdl;
	uint32 color_type_size;

	if( bmp_file == NULL )
		return NULL;
		
	fp = g_fopen( bmp_file, "rb+" );
	if( fp == NULL )
		return NULL;

	hdl = (HBMP_i)g_malloc( sizeof(HBMP_i_t) );
	if( hdl == NULL )
	{
		g_fclose( fp );
		return NULL;
	}
	
	/* get bmp file head */
	g_fread( &file_head, 1, sizeof(file_head), fp );	
	if( file_head.bfType[0] != 'B' || file_head.bfType[1] != 'M' )
		goto error;
	if( file_head.bfOffBits != sizeof(bmp_file_head_t) + sizeof(bmp_info_head_t) )
		goto error;
	
	/* get bmp information head */
	g_fread( &info_head, 1, sizeof(info_head), fp );
	if( info_head.biBitCount != 24 && info_head.biBitCount != 32 )
		goto error;
	
	hdl->fp = fp;
	hdl->width  = info_head.biWidth ;
	hdl->height = info_head.biHeight;
	hdl->bitcount = info_head.biBitCount;
	if( info_head.biBitCount == 24 )
		hdl->color_mode = BMP_COLOR_MODE_24BITS;
	else if( info_head.biBitCount == 32 )
		hdl->color_mode = BMP_COLOR_MODE_32BITS;
	else
		goto error;
	color_type_size = hdl->bitcount >> 3;
	hdl->color_type_size = color_type_size;
	hdl->row_size = ( hdl->width * color_type_size + 4 - 1 ) & (~( 4 - 1 ) );

	return (HBMP)hdl;

error:
	g_fclose( fp );
	return NULL;
}




int32 close_bmp( HBMP handle )
{
	HBMP_i hdl = (HBMP_i)handle;
	
	if( hdl == NULL )
		return -1;
		
	g_fclose( hdl->fp );
	g_free( hdl );
	
	return 0;
}




/* origin is lower left corner. */
/*
int32 draw_pixel_llc( HBMP handle, uint32 x, uint32 y, const full_color_t *color_p )
{
	uint32 offset;
	HBMP_i hdl = (HBMP_i)handle;
		
	if( x >= hdl->width || y >= hdl->height )
		return -1;
		
	offset = sizeof(bmp_file_head_t) + sizeof(bmp_info_head_t) + y * hdl->row_size + 3 * x;
	g_fseek( hdl->fp, offset, SEEK_SET );
	g_fwrite( color_p, 1, 3, hdl->fp );
	
	return 0;
}
*/



int32 draw_pixel( HBMP handle, uint32 x, uint32 y, const full_color_t *color_p, int32 origin_pos )
{
	uint32 offset;
	uint32 color_type_size;
	HBMP_i hdl = (HBMP_i)handle;
	
	if( handle == NULL )
		return -1;
		
	if( x >= hdl->width || y >= hdl->height )
		return -1;
		
	color_type_size = hdl->color_type_size;
	if( origin_pos == ORIGIN_POS_UPPER_LEFT )	
		offset = sizeof(bmp_file_head_t) + sizeof(bmp_info_head_t) + ( hdl->height - 1 - y )* hdl->row_size + color_type_size * x;
	else if( origin_pos == ORIGIN_POS_LOWER_LEFT )
		offset = sizeof(bmp_file_head_t) + sizeof(bmp_info_head_t) + y * hdl->row_size + color_type_size * x;
	else
		return -1;
		
	g_fseek( hdl->fp, offset, SEEK_SET );
	g_fwrite( color_p, 1, color_type_size, hdl->fp );
	
	return 0;
}



int32 read_pixel( HBMP handle, uint32 x, uint32 y, full_color_t *color_p, int32 origin_pos )
{
	uint32 offset;
	uint32 color_type_size;
	HBMP_i hdl = (HBMP_i)handle;
	
	if( handle == NULL )
		return -1;
		
	if( x >= hdl->width || y >= hdl->height )
		return -1;
		
	color_type_size = hdl->color_type_size;
	if( origin_pos == ORIGIN_POS_UPPER_LEFT )	
		offset = sizeof(bmp_file_head_t) + sizeof(bmp_info_head_t) + ( hdl->height - 1 - y )* hdl->row_size + color_type_size * x;
	else if( origin_pos == ORIGIN_POS_LOWER_LEFT )
		offset = sizeof(bmp_file_head_t) + sizeof(bmp_info_head_t) + y * hdl->row_size + color_type_size * x;
	else
		return -1;
		
	g_fseek( hdl->fp, offset, SEEK_SET );
	g_fread( color_p, 1, color_type_size, hdl->fp );
	
	return 0;
}






#endif     //  ifndef __bmp_c

/* end of bmp.c  */
