//
//  permutation.h
//  CETD_128
//
//  Created by Duke on 5/03/13.
//  Copyright (c) 2013 Duke. All rights reserved.
//

#ifndef CETD_128_permutation_h
#define CETD_128_permutation_h
#include "config.h"
#define SHIFT_P 20
#define SHIFT_P_P 5
#define SHUFFLE_P 19

int mod(int a, int b);

void rotate_p(const uchar *nonce,
		uchar *s,
		int y_num,
		int tag_length,
		int shuffle_r,
		int s_p,
		int r_p);

void permutation(uchar **data,
		uchar *s,
		int number, 
		int arr_length);
void serial_permutation(uchar **data,
		uchar *s,
		int number, 
		int arr_length, uchar *tag);


/*
void shift_p(uchar *nonce, 
		uchar *s,
		int r,
		int shuffle_p,
		int shift_p);
*/
#endif
