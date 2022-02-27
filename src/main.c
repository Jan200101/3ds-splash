#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "image.h"

#define ARRAY_LEN(arr) sizeof(arr) / sizeof(arr[0])

#define FORMAT_ERROR "%s is not in a supported format\n"

typedef size_t (*splash_func)(uint8_t**, size_t);
typedef struct {
    char* extension;
    splash_func func;
} splash_types;

const splash_types in_conversions[] = {
    { .extension = "bin", .func = bin_to_rgba },
    { .extension = "png", .func = png_to_rgba },
};

const splash_types out_conversions[] = {
    { .extension = "bin", .func = rgba_to_bin },
    { .extension = "png", .func = rgba_to_png },
};

int main(int argc, char** argv)
{
    FILE* in_fd = NULL;
    FILE* out_fd = NULL;

    int retval = EXIT_SUCCESS;
    if (argc < 3)
    {
        fprintf(stderr, "%s [input] [output]\n", argv[0]);
        retval = EXIT_FAILURE;
        goto exit;
    }

    char* in_path = argv[1];
    size_t in_path_len = strlen(in_path);
    splash_func in_func = NULL;

    char* out_path = argv[2];
    size_t out_path_len = strlen(out_path);
    splash_func out_func = NULL;

    for (size_t i = 0; i <= ARRAY_LEN(in_conversions); ++i)
    {
        if (i == ARRAY_LEN(in_conversions))
        {
            fprintf(stderr, FORMAT_ERROR, in_path);
            retval = EXIT_FAILURE;
            goto exit;
        }

        char* extension = in_conversions[i].extension;
        if (!strcmp(in_path+(in_path_len-strlen(extension)), extension))
        {
            in_func = in_conversions[i].func;
            break;
        }
    }

    for (size_t i = 0; i <= ARRAY_LEN(out_conversions); ++i)
    {
        if (i == ARRAY_LEN(out_conversions))
        {
            fprintf(stderr, FORMAT_ERROR, out_path);
            retval = EXIT_FAILURE;
            goto exit;
        }

        char* extension = out_conversions[i].extension;
        if (!strcmp(out_path+(out_path_len-strlen(extension)), extension))
        {
            out_func = out_conversions[i].func;
            break;
        }
    }


    in_fd = fopen(in_path, "rb");
    if (!in_fd)
    {
        fprintf(stderr, "cannot open %s\n", in_path);
        retval = EXIT_FAILURE;
        goto exit;
    }

    fseek(in_fd, 0L, SEEK_END);
    size_t in_size = (size_t)ftell(in_fd);
    rewind(in_fd);

    if (!in_size)
    {
        fprintf(stderr, "%s is empty\n", in_path);
        retval = EXIT_FAILURE;
        goto exit;
    }

    out_fd = fopen(out_path, "wb");
    if (!out_fd)
    {
        fprintf(stderr, "cannot open %s\n", out_path);
        retval = EXIT_FAILURE;
        goto exit;
    }

    uint8_t* buf = malloc(in_size+1);
    fread(buf, 1, in_size, in_fd);
    buf[in_size] = '\0';

    if (in_func)
    {
        in_size = in_func(&buf, in_size);
    }

    if (out_func)
    {
        in_size = out_func(&buf, in_size);
    }

    fwrite(buf, 1, in_size, out_fd);

    exit:

    if (in_fd) fclose(in_fd);
    if (out_fd) fclose(out_fd);

    return retval;
}