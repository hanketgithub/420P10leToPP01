/**
  * Convert P10le (16 bit for 10 bit) to PP01 (packed 10 bit, 10 bit for 10 bit)
  *
  */

#include <stdint.h>

#include "pack.h"

/**
  *         LSB                                 MSB
  *  pix0   00  01  02  03  04  05  06  07   08  09
  *         <----------data[0]----------->  <data[1]>
  *
  *  pix0: [00...07] = y[0], [08, 09] = y[1] & 0x03;
  *
  *
  *  pix1   10  11  12  13  14  15   16  17  18  19
  *         <-------data[1]------>   <---data[2]--->
  *
  *  pix1: [10...17] = y[2], [18, 19] = y[3] & 0x03;
  *
  *
  *  pix2   20  21  22  23   24  25  26  27  28  29
  *         <---data[2]--->  <------data[3]------->
  *
  *  pix2: [20...27] = y[4], [28, 29] = y[5] & 0x03;
  *
  *
  *  pix3   30  31   32  33  34  35  36  37  38  39
  *        <data[3]> <----------data[4]------------>
  *
  *  pix3: [30...37] = y[6], [38, 39] = y[7] & 0x03;
  *
  *
  */
void pack_y(QUATRE_PIX_T *q_pix, uint8_t *y)
{    
    //
    // y[0] has 8 valid bits
    // so data[0] = y[0]
    //
    q_pix->data[0] = y[0];
    
    //
    // y[1] has 2 valid bits
    // data[1] filled with y[1], and it(data[1]) has 6 bits space left
    //
    // y[2] has 8 valid bits
    // Let 6 out of them goes into data[1]:
    //
    // 07 06 05 04 03 02   01  00
    // < y[2] & 3f << 2>   <y[1]>
    //
    q_pix->data[1] = (y[1]) |       // y[1] valid 2 bit
                     ((y[2] & 0x3F) << 2); // 6 bit

    //
    // Now y[2] has 2 bits left,
    // y[3] has 2 valids bits,
    // y[4] has 8 valid bits, fit 4 bits in data[2]
    // 
    // 07   06   05   04   03  02   01  00
    // <y[4] & 0xf << 4>   <y[3]>   <y[2]>
    //    
    q_pix->data[2] = (y[2] >> 6) |  // 2 bit
                     (y[3] << 2) |  // 2 bit
                     (y[4] << 4);   // 4 bit

    //
    // Now y[4] has 4 valid bit left fit 4 bits in data[3]
    // y[5] has 2 valid bits
    // data[3] has 2 more space left, fit 2 bits from y[6]
    //
    // 07  06   05  04   03  02 01  00
    // <y[6]>   <y[5]>   <y[4] & 0xf0>
    //
    q_pix->data[3] = (y[4] >> 4) |  // 4 bit
                     (y[5] << 4) |  // 2 bit
                     (y[6] << 6);   // 2 bit
                    
    //
    // Now y[6] has 6 valid bits left, fit in data[4]
    // y[7] has 2 valid bits.
    //
    // 07  06   05  04 03 02 01 00
    // <y[7]>   <y[6] & 0xfc >> 2>
    //
    q_pix->data[4] = (y[6] >> 2) |
                     (y[7] << 6);
}


/**
  *           LSB                                 MSB
  *  pix u0   00  01  02  03  04  05  06  07   08  09
  *           <----------data[0]----------->  <data[1]>
  *
  *  pix u0: [00...07] = u[0], [08, 09] = u[1] & 0x03;
  *
  *
  *  pix v0   10  11  12  13  14  15   16  17  18  19
  *           <-------data[1]------>   <---data[2]--->
  *
  *  pix v0: [10...17] = v[0], [18, 19] = v[0] & 0x03;
  *
  *
  *  pix u1   20  21  22  23   24  25  26  27  28  29
  *           <---data[2]--->  <------data[3]------->
  *
  *  pix u1: [20...27] = u[2], [28, 29] = u[3] & 0x03;
  *
  *
  *  pix v1   30  31   32  33  34  35  36  37  38  39
  *           <data[3]> <----------data[4]---------->
  *
  *  pix v1: [30...37] = v[2], [38, 39] = v[3] & 0x03;
  *
  *
  */
void pack_uv(QUATRE_PIX_T *q_pix, uint8_t *u, uint8_t *v)
{    
    // data[0]
    q_pix->data[0] = u[0];
    
    // data[1]
    q_pix->data[1] = (u[1]) |
                     (v[0] << 2);

    // data[2]
    q_pix->data[2] = (v[0] >> 6) |  // 2 bit
                     (v[1] << 2) |  // 2 bit
                     (u[2] << 4); // 4 bit

    // data[3]
    q_pix->data[3] = (u[2] >> 4) |  // 4 bit
                     (u[3] << 4) |  // 2 bit
                     (v[2] << 6);   // 2 bit
                    
    // data[4]
    q_pix->data[4] = (v[2] >> 2) |
                     (v[3] << 6);
}

