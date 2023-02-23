#ifndef __PACK_TEN_BIT__
#define __PACK_TEN_BIT__

//
// A placeholder to store four P10le pixels in PP01 format.
//
// A P10le pixel has two bytes, therefore 4 P10le pixels have 8 bytes.
// Store them in PP01 format will take 4 x 10 bits = 40 bits = 5 bytes.
//
typedef struct
{
    uint8_t data[5];
} QUATRE_PIX_T;


void pack_y(QUATRE_PIX_T *q_pix, uint8_t *y);

void pack_uv(QUATRE_PIX_T *q_pix, uint8_t *u, uint8_t *v);

void framecopy_I0AL_to_PP01(uint32_t width, uint32_t height, uint8_t *srcmem[], uint8_t *dstmem);

size_t framesize_PP01(uint32_t width, uint32_t height);

#endif
