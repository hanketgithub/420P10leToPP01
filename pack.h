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
} quatre_pixel;


void pack_y(quatre_pixel *q_pix, uint8_t *y);

void pack_uv(quatre_pixel *q_pix, uint8_t *u, uint8_t *v);
#endif