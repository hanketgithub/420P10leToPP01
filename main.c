//
//  main.c
//  420P10ToP010
//
//  Created by Hank Lee on 2/18/23.
//  Copyright (c) 2023 Hank Lee. All rights reserved.
//

#include <stdint.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include "pack.h"

#define SSE 1

#define MAX_WIDTH   7680
#define MAX_HEIGHT  4320


static uint8_t img[MAX_WIDTH * MAX_HEIGHT * 3];

int main(int argc, char *argv[])
{
    uint64_t *y;
    uint32_t *u;
    uint32_t *v;
    QUATRE_PIX_T q_pix;
    int ifd, ofd;

    int width;
    int height;
    int wxh;
    
    int i, j;
    int count = 0;
    ssize_t rd_sz;
    
    char *cp;
    char output[256] = { 0 };
    
    if (argc < 4)
    {
        fprintf(stderr, "usage: %s [input file] [width] [height]\n", argv[0]);
        
        return -1;
    }

    cp = NULL;

    // get input file name from comand line
    ifd = open(argv[1], O_RDONLY);
    if (ifd < 0)
    {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    // specify output file name
    cp = strrchr(argv[1], '.');
    strncpy(output, argv[1], cp - argv[1]);
    strcat(output, "_pp01");
    strcat(output, cp);
    
    ofd = open(output, O_RDWR | O_CREAT, S_IRUSR);

    width   = atoi(argv[2]);
    height  = atoi(argv[3]);
    wxh     = width * height;

 
    while ((rd_sz = read(ifd, img, wxh * 3)) == wxh * 3)
    {
        #if (SSE)
        uint8_t *srcmem[2] = { img, img + wxh * 2 }; // { Y, U, V }
        uint8_t *dstmem = (uint8_t *) calloc(1, framesize_PP01(width, height));

        framecopy_I0AL_to_PP01(width, height, srcmem, dstmem);

        write(ofd, dstmem, framesize_PP01(width, height));
        free(dstmem);
        #else
        y = (uint64_t *) img;
        u = (uint32_t *) ((uint8_t *) img + wxh * 2);
        v = (uint32_t *) ((uint8_t *) img + wxh * 2 + wxh * 2 / 4);

        // Y
        QUATRE_PIX_T *py = calloc(1, wxh / 4 * sizeof(QUATRE_PIX_T));

        QUATRE_PIX_T *p = py;
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width / 4; j++) // 4 horizantal pixel at a time
            {
                memset(&q_pix, 0, sizeof(q_pix));

                pack_y(&q_pix, (uint8_t *) y);

                memcpy(p, &q_pix, sizeof(QUATRE_PIX_T));

                y++;
                p++;
            }
        }
        write(ofd, py, wxh / 4 * sizeof(QUATRE_PIX_T));
        free(py);
        
        // U, V
        QUATRE_PIX_T *quv = calloc(1, wxh / 2 / 4 * sizeof(QUATRE_PIX_T));
        QUATRE_PIX_T *q = quv;
        for (i = 0; i < height / 2; i++)
        {
            for (j = 0; j < width / 4; j++) // 2 horizontal pixel at a time
            {
                memset(&q_pix, 0, sizeof(q_pix));
                
                pack_uv
                (
                    &q_pix,
                    (uint8_t *) u,
                    (uint8_t *) v
                );

                memcpy(q, &q_pix, sizeof(QUATRE_PIX_T));

                u++;
                v++;
                q++;
            }
        }
        write(ofd, quv, wxh / 2 / 4 * sizeof(QUATRE_PIX_T));
        free(quv);
        #endif
        
        fprintf(stderr, "Frame %d completed.\n", count);
        count++;
    }
    
    close(ifd);
    close(ofd);
    
    fprintf(stderr, "Done\n");
    fprintf(stderr, "Output file: %s\n", output);
    
    return 0;
}
