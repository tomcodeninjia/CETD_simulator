//original shuffle 

#include "config.h"
#define SHUFFLE_P 19
#define SEG_BITS 32
void split_test(uint x);
void debug(unsigned int x);
void shuffle_p(uchar *nonce, uint *v, int r);
void shift_p(uchar *nonce, int *s,int r);
void swap(uchar *nonce, uchar **data, int r,int number, int arr_length);
void split_test(uint x);
//void swap1(unsigned short *mask,uchar *input,int r);
