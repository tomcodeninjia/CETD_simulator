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
void shift_p(uchar *nonce, int *s,int r);
void permutation(int s, uchar **data, int number, int arr_length);
#endif
