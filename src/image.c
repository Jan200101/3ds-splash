#include <stdlib.h>
#include <png.h>

#include "image.h"

size_t bin_to_rgba(uint8_t** bufp, size_t size)
{
    size_t out_size = (size / 3) * 4;
    uint8_t* buf = malloc(out_size);

    for (size_t i = 0, j = 0; i < size; i+=3, j+=4)
    {
        (buf+j)[0] = (*bufp+i)[2];
        (buf+j)[1] = (*bufp+i)[1];
        (buf+j)[2] = (*bufp+i)[0];
        (buf+j)[3] = 0xFF;
    }
    free(*bufp);
    *bufp = buf;
    return out_size;
}


size_t rgba_to_bin(uint8_t** bufp, size_t size)
{
    size_t out_size = (size / 4) * 3;
    uint8_t* buf = malloc(out_size);

    for (size_t i = 0, j = 0; i < size; i+=4, j+=3)
    {
        (buf+j)[0] = (*bufp+i)[2];
        (buf+j)[1] = (*bufp+i)[1];
        (buf+j)[2] = (*bufp+i)[0];
    }
    free(*bufp);
    *bufp = buf;
    return out_size;
}

size_t png_to_rgba(uint8_t** bufp, size_t size)
{
    size_t out_size = 0;

    uint32_t* buf = (uint32_t*)*bufp;
    FILE* fp = fmemopen(buf, size, "rb");;
    png_bytep* row_pointers = NULL;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);

    png_init_io(png, fp);
    png_read_info(png, info);

    if(png_get_bit_depth(png, info) == 16) png_set_strip_16(png);

    uint32_t w = png_get_image_width(png, info);
    uint32_t h = png_get_image_height(png, info);

    printf("%i, %i\n", w, h);

    png_byte color_type = png_get_color_type(png, info);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
 
    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    row_pointers = malloc(sizeof(png_bytep) * h);
    out_size = sizeof(uint32_t) * (w * h);
    uint32_t* out = malloc(out_size);
    for(uint32_t y = 0; y < h; y++)
    {
        row_pointers[y] = (png_byte*)(out+(w*y));
    }

    png_read_image(png, row_pointers);

    png_destroy_read_struct(&png, &info, NULL);

    if (fp) fclose(fp);
    if (row_pointers) free(row_pointers);

    free(*bufp);
    *bufp = (uint8_t*)out;

    return out_size;
}

#define DEFAULT_WIDTH 240

size_t rgba_to_png(uint8_t** bufp, size_t size)
{
    uint32_t* buf = (uint32_t*)*bufp;

    size_t out_size = size;
    uint32_t* out = malloc(out_size);
    memset(out, 0, out_size);
    //FILE *fp = fopen("out.png", "wb");
    FILE* fp = fmemopen(out, size, "wb");
    png_bytep* row_pointers = NULL;

    uint32_t w = 240;
    uint32_t h = (uint32_t)((size/4)/w);

    printf("%i, %i\n", w, h);

    printf("%li\n", size);

    /* initialize stuff */
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png);

    png_init_io(png, fp);

    png_set_IHDR(png, info_ptr, w, h,
                 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info_ptr);

    row_pointers = malloc(sizeof(png_bytep) * h);
    for(uint32_t y = 0; y < h; y++)
    {
        row_pointers[y] = (png_byte*)(buf+(w*y));
    }

    png_write_image(png, row_pointers);

    png_write_end(png, NULL);

    free(row_pointers);
    fclose(fp);

    while (!out[out_size/4]) --out_size;
    
    free(buf);
    *bufp = out;

    return out_size;
}