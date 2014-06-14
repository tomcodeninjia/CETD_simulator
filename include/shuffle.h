//original shuffle 

#include "config.h"
#define SHUFFLE_P 19
#define SEG_BITS 32
void split_test(uint x);
void debug(unsigned int x);
void split_test(uint x);
//void shift_p(uchar *nonce, int *s,int r);

void swap_p(const uchar *nonce, 
		uint *v, 
		int r,
		int shuffle_p);

void swap(const uchar *nonce, 
		uchar **data, 
		int r, 
		int shuffle_p,
		int number, int arr_length//y_num and tag_len
		);

void swap_with_nonce(const uchar *nonce, 
		uchar **input, 
		int round, 
		int shuffle_para_len,
		int number, int arr_length//y_num and tag_len
		);

//void swap1(unsigned short *mask,uchar *input,int r);
