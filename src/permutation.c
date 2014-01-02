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
void shift_p(uchar *nonce, int *s,int r)
{
    uint k=0;
    uint blk = (r*SHUFFLE_P+SHIFT_P)/CHAR_BIT - r*SHUFFLE_P/CHAR_BIT +1;
    //printf("blk %d \n",blk);
    k = (*(nonce + (r*SHUFFLE_P + SHIFT_P)/CHAR_BIT)) >> (CHAR_BIT-((SHIFT_P + (r * SHUFFLE_P)%CHAR_BIT)%CHAR_BIT));
    //printf("start is %d, end is %d \n",r*SHUFFLE_P/CHAR_BIT , (r*SHUFFLE_P+SHIFT_P)/CHAR_BIT);
    //printf("k is %x\n",k);
    for(int j=0;j<blk;j++)
    {
          //printf("j is %d\n",j);
        //printf("block is %x \n",blk);
        k |= (*(nonce + (r*SHUFFLE_P+SHIFT_P)/CHAR_BIT-j-1)) << ((((r*SHUFFLE_P + SHIFT_P))%CHAR_BIT) + j*CHAR_BIT);
         //int l= (((r*SHUFFLE_P + SHIFT_P) % CHAR_BIT) + j*CHAR_BIT) ;
         //printf("l is %x \n",l);
      //   printf("k is %x\n",k);
    }
    *s = k & ((1 << SHIFT_P)-1);
    //printf("s is %x \n",*s);
    
}

void permutation(int s, uchar **data, int number, int arr_length)
{
    
    //printf("s is %x \n",s);
    for(int i=0;i<number;i++)
    {
        int shift= (s & (1 << (i+1)*SHIFT_P_P)-1)>> i*SHIFT_P_P ;
       
        /*
        printf("data before shift\n");
        for(int i=0;i<4;i++)
        {
          show((uchar*)data + i * BLK_LENGTH, BLK_LENGTH);
        }
       
         */
        //printf("shift is %d\n",shift);
        shiftrrn(data[i], arr_length, shift);
        /*
        printf("data after shift\n");
        for(int i=0;i<4;i++)
        {
            show((uchar*)data + i * BLK_LENGTH, BLK_LENGTH);
      }
*/ 
     
        
    }
}

