//
//  CETD.c
//  CETD_128
//
//  Created by Duke on 5/03/13.
//  Copyright (c) 2013 Duke. All rights reserved.
//

#include <stdlib.h>
#include "../include/CETD.h"
#include "../include/show.h"
#include "../include/write_txt.h"
#include "../include/write_csv.h"
#include <time.h>

#define Y_SINGLE BLK_LENGTH/TAG_LENGTH
//input:data,nonce,r,
void CETD_tag_generation(const uchar **data, uchar *nonce_input,  aes_context a_ctx,int r, int s,  FILE *x,FILE *y1[Y_NUMBER], FILE *y2[Y_NUMBER], FILE *CETD_tag, bool file_type)
{
	uchar CETD_data[Y_NUMBER][TAG_LENGTH];	
	int i,j;
	for( i=0;i<Y_NUMBER;i++)
	{
		for( j=0;j<TAG_LENGTH;j++)
		{
			uchar x=i-(i/Y_SINGLE)*Y_SINGLE;
			CETD_data[i][j]=data[i/Y_SINGLE][Y_SINGLE*x+j];
		}
	}
		
    uchar *nonce;
    
    nonce=(uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
    memset(nonce, 0, BLK_LENGTH);
    
    aes_crypt_ecb(&a_ctx, AES_ENCRYPT,nonce_input, nonce);
    
    //shuffle data
    uchar **shift_data=(uchar **)malloc(sizeof(uchar *)*Y_NUMBER);
    for(int i=0;i<Y_NUMBER;i++)
    {
        shift_data[i]=(uchar *)malloc(sizeof(uchar)*TAG_LENGTH);

        memset(shift_data[i], 0, TAG_LENGTH);
    }
      
    for(int i=0;i<Y_NUMBER;i++)
    {
        for(int j=0;j<TAG_LENGTH;j++)
        {
            *(*(shift_data+i)+j)= CETD_data[i][j];
        }
    }
    
        
    shift_p(nonce, &s, r);
    permutation(s, shift_data, Y_NUMBER, TAG_LENGTH);
	/*
	 *need to store BLK_NUMBER files for y1
	 * */
	for(int i=0;i<Y_NUMBER;i++)
	{
		if(file_type==TXT_file)
		{
			write_txt_1array(y1[i],TAG_LENGTH,shift_data[i]);
		}	
		else
		{
			write_csv_1array(y1[i],TAG_LENGTH,shift_data[i]);
		}	

	}

    //the swap_data is the output of 
	//shuffle data
    uchar **swap_data=(uchar **)malloc(sizeof(uchar *)*Y_NUMBER);
    for(int i=0;i<Y_NUMBER;i++)
    {
        swap_data[i]=(uchar *)malloc(sizeof(uchar)*TAG_LENGTH);

        memset(swap_data[i], 0, TAG_LENGTH);
    }
      
    for(int i=0;i<Y_NUMBER;i++)
    {
        for(int j=0;j<TAG_LENGTH;j++)
        {
            *(*(swap_data+i)+j)= CETD_data[i][j];
        }
    }

    swap(nonce,swap_data,r,Y_NUMBER,TAG_LENGTH);

	if(file_type==TXT_file){
		write_txt_2array(x,Y_NUMBER,TAG_LENGTH,swap_data);
	}
	else
	{
		write_csv_2array(x,Y_NUMBER,TAG_LENGTH,swap_data);
	}

        
    shift_p(nonce, &s, r);
    permutation(s, swap_data, Y_NUMBER, TAG_LENGTH);

    for(int i=0;i<Y_NUMBER;i++)
	{
		if(file_type==TXT_file)
		{
			write_txt_1array(y2[i],TAG_LENGTH,swap_data[i]);
		}	
		else
		{
			write_csv_1array(y2[i],TAG_LENGTH,swap_data[i]);
		}	

	}

	uchar *tag=(uchar *)malloc(sizeof(uchar)*TAG_LENGTH);
	memset(tag, 0, TAG_LENGTH);

    tag_gene(swap_data, tag, Y_NUMBER, TAG_LENGTH);
    
	
	if(file_type==TXT_file)
	{
		write_txt_1array(CETD_tag,TAG_LENGTH,tag);
	}
	else
	{
		write_csv_1array(CETD_tag,TAG_LENGTH,tag);
	}
   
    //printf("tag is\n");
    //show(tag,BLK_LENGTH);
    
}
