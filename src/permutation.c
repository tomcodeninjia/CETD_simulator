//
//  permutation.c
//  CETD_128
//
//  Created by Duke on 5/03/13.
//  Copyright (c) 2013 Duke. All rights reserved.
//

#include <stdlib.h>
#include "../include/permutation.h"
#include <limits.h>
#include "../include/array_shift.h"
#include "../include/show.h"

int mod(int a, int b)
{
	return a - (a/b)*b;
}
void rotate_p(const uchar *nonce,
		uchar *s, //rotate para array
		int y_num,
		int tag_length,
		int shuffle_r, // shuffle rounds
		int s_p, //length of shuffle para
		int r_p) //length of rotate para
{
	int start,end;
	uchar tmp;	
	uchar b;
	int shift_index;
	for(int i=0;i< y_num;i++)
	{
		start = (s_p * shuffle_r + i * r_p)/CHAR_BIT;
		end = (shuffle_r*s_p + i*r_p + r_p)/CHAR_BIT;
//		printf("%d, %d\n", start, end);

		//r*s_p + i * r_p + r_p % CHAR_BIT	
		tmp = nonce[end];
		shift_index = (CHAR_BIT - mod(shuffle_r*s_p + i*r_p + r_p, CHAR_BIT));
		if(start != end)
		{
			b =  (tmp >> shift_index)  &  ((1 << shift_index )- 1); 
			tmp = nonce[start];
			b |= tmp << (CHAR_BIT - shift_index);   

			b = b & ((1 << r_p) - 1);

		}
		else
		{
			b = (tmp >> shift_index) & ((1 << r_p)-1);
		}
				*(s+i)=mod(b,tag_length*CHAR_BIT);
//		printf("%d,%d\n",b, *(s+i));
	}
}

void serial_permutation(uchar **data,
		uchar *s,
		int number, 
		int arr_length, uchar *tag)
{

	uchar *tmp_arr;
	tmp_arr = (uchar *)malloc(sizeof(uchar)*arr_length);
	for(int i=0;i<number;i++)
	{
		*(tmp_arr+i) = 0;
	}
	
	for(int i=0;i<number;i++)
    {
		for(int j=0;j<arr_length;j++)	
		{
			tmp_arr[j] ^= data[i][j];	
		}
        shiftrrn(tmp_arr, arr_length, s[i]);
    }
	for(int i=0;i<arr_length;i++)
	{
		*(tag+i) = tmp_arr[i];
	}
	free(tmp_arr);

}
void permutation(uchar **data,
		uchar *s,
		int number, 
		int arr_length)
{
    
    for(int i=0;i<number;i++)
    {
        shiftrrn(data[i], arr_length, s[i]);
    }
}

/*
void shift_p(uchar *nonce, 
		uchar *s,
		int r,
		int shuffle_p,
		int shift_p)
{
    uint k=0;
	//blk: rotate parameter length 
    uint blk = (r*shuffle_p+shift_p)/CHAR_BIT - r*shuffle_p/CHAR_BIT +1;
	//k: the value of last byte for rotate para
    k = (*(nonce + (r*shuffle_p + shift_p)/CHAR_BIT)) >> (CHAR_BIT-((shift_p + (r * shuffle_p)%CHAR_BIT)%CHAR_BIT));
    //printf("k is %x\n",k);
    for(int j=0;j<blk;j++)
    {
          //printf("j is %d\n",j);
        //printf("block is %x \n",blk);
        k |= (*(nonce + (r*shuffle_p+shift_p)/CHAR_BIT-j-1)) << ((((r*shuffle_p + shift_p))%CHAR_BIT) + j*CHAR_BIT);
         //int l= (((r*shuffle_p + shift_p) % CHAR_BIT) + j*CHAR_BIT) ;
         //printf("l is %x \n",l);
      //   printf("k is %x\n",k);
    }
    *s = k & ((1 << shift_p)-1);
    //printf("s is %x \n",*s);
    
}
*/
