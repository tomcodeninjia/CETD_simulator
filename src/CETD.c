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
//void CETD_nonce_input(uint addr, uint counter, );
#define Y_SINGLE block_length/tag_length
//input:data,nonce,r,


//length of S[i]
int shuffle_p(int y_num,int tag_length)
{
	return 3*log2_int(tag_length)+2*log2_int(y_num);		
}

/*
 split addr and store to uchar *addr_arr
 @para
 @para
 */
void addr_arr_generation(int addr, 
		int addr_len, 
		uchar *nonce) // |addr_seg, crt_seg|, |crt_byte|
{

	int tmp_addr = addr;
	if(addr_len > sizeof(int)*CHAR_BIT)
	{
		int i=0;
		while(tmp_addr != 0)
		{
			*(nonce+i) = tmp_addr & ((1 << CHAR_BIT) - 1);	
			tmp_addr = tmp_addr >> CHAR_BIT;
		}
	}
	else
	{
		int tmp_len = addr_len;
		int i=0;
		while( tmp_len >0)
		{
			if(tmp_len > CHAR_BIT)
			{
				*(nonce+i) = tmp_addr & ((1 << CHAR_BIT) - 1);	
			}
			else
			{
				*(nonce + i) = (tmp_addr & ((1 << tmp_len) - 1)) << (CHAR_BIT - tmp_len); 			
			}
			i++;
			tmp_len -= CHAR_BIT;
			tmp_addr = tmp_addr >> CHAR_BIT;
		}
	}
	 
}

/*
 split crt to uchar *crt_arr
 @para
 @para
 */
void crt_arr_generation(int crt, 
		int crt_len, 
		uchar *nonce,
		int addr_len )// |addr_seg, crt_seg|,|crt_byte|.. 
{
	int tmp_crt = crt;
	uchar b = tmp_crt & ((1 << (CHAR_BIT - mod(addr_len,CHAR_BIT))) -1);
	nonce[addr_len/CHAR_BIT] |= b;
	tmp_crt = tmp_crt >> (CHAR_BIT - mod(addr_len,CHAR_BIT));

	int i=1;
	if(crt_len > sizeof(int)*CHAR_BIT)
	{
		while(tmp_crt != 0)
		{
			*(nonce+i+addr_len/CHAR_BIT) = tmp_crt & ((1 << CHAR_BIT) - 1);	
			tmp_crt = tmp_crt >> CHAR_BIT;
		}	
		
	}
	else
	{
		int tmp_len = crt_len;
		while( tmp_len >0)
		{
			if(tmp_len > CHAR_BIT)
			{
				*(nonce+i + addr_len/CHAR_BIT) = tmp_crt & ((1 << CHAR_BIT) - 1);	
			}
			else
			{
				*(nonce + i+ addr_len/CHAR_BIT) = (tmp_crt & ((1 << tmp_len) - 1)) << (CHAR_BIT - tmp_len); 			
			}
			i++;
			tmp_len -= CHAR_BIT;
			tmp_crt = tmp_crt >> CHAR_BIT;
		}
	}

	
}

void rnd_arr_generation(const uchar *rnd, 
		int crt_len, 
		int addr_len,
		uchar *nonce)// |addr_seg, crt_seg|,|crt_byte|.. 
{
	
	if((addr_len + crt_len)!= BLK_LENGTH * CHAR_BIT)
	{
		uchar tmp_r = rnd[0];
		uchar b = tmp_r & ((1 << (CHAR_BIT - mod(addr_len+crt_len,CHAR_BIT))) -1);
		nonce[(addr_len+crt_len)/CHAR_BIT] |= b;

		int i=1;	
		int start = (addr_len+crt_len)/CHAR_BIT +1;
		while(start<BLK_LENGTH)
		{
			*(nonce+start) = rnd[i];
			start++;
			i++;
		}
		/*
		for(start;start < BLK_LENGTH;start++,i++)
		{
			*(nonce+start) = rnd[i];
		}		
		*/
	}

}

/*
 use addr_arr, crt_arr and rnd_arr to form nonce_input
 @para 
 @para
 */
void nonce_input_generation(uchar *nonce_input, 
		int addr, int addr_len,
		int crt, int crt_len,
		uchar *rnd)
{

	//printf("addr %x\n", addr);
 	addr_arr_generation( addr, 
		 addr_len, 
		nonce_input); // |addr_seg, crt_seg|, |crt_byte|

	//printf("crt %x\n", crt);
	crt_arr_generation( crt, 
		 crt_len, 
		nonce_input,
		 addr_len );// |addr_seg, crt_seg|,|crt_byte|.. 

 	rnd_arr_generation(rnd, 
		 crt_len, 
		 addr_len,
		nonce_input);// |addr_seg, crt_seg|,|crt_byte|.. 


}

/*
 CETD simulator
 @para data : No. of blks
 @para block_num: ciphertext blk array, 
 @para a_ctx: aes_context for blk cipher

 @para tag_length : len of tag
 @para y_num: block_num * block_length / tag_len
 @para r: shuffle rounds
 @para nonce_input: (A, C,R)

 @para x, y1, y2 : internal output
 @para 
 */
void CETD_tag_generation(const uchar **data,int block_num, 
		int block_length,
		uchar *nonce_input, 
		aes_context a_ctx,
		int r,  //shuffle round  
		int tag_length,// No. of Byte
		int y_num, // No. of internal Y blks
		FILE *x,
		FILE **y1, 
		FILE **y2, 
		FILE *CETD_tag, 
		FILE *CETD_nonce,
		bool file_type,
		bool result_format)
{

	/*
	 concatenate the data array to a 1D array
	 */
	uchar *tmp_data = (uchar *)malloc(sizeof(uchar)*block_length*block_num);
	for(int i=0;i<block_length*block_num;i++)
	{
		uchar x = i - (i/block_length)*block_length;
		tmp_data[i]=data[i/block_length][x];
	}

	
	uchar **CETD_data =(uchar **)malloc(sizeof(uchar *)*y_num);	
	for(int i=0;i<y_num;i++)
	{
		CETD_data[i]=(uchar *)malloc(sizeof(uchar)*tag_length);
		memset(CETD_data[i],0,tag_length);
	}

	for(int i=0;i<y_num-1;i++)
	{
		for(int j=0;j<tag_length;j++)
		{
			*(*(CETD_data+i)+j) = tmp_data[i*tag_length+j];
		}
	}
	int remain = block_length*block_num - tag_length*(y_num-1);
	for(int i=0;i<remain;i++)
	{
		*(*(CETD_data+y_num-1)+i) = tmp_data[block_length*block_num-remain+i];
	}

	/*
	 generate nonce
	 */
    uchar *nonce;
    
    nonce=(uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
    memset(nonce, 0, BLK_LENGTH);
    
	
    aes_crypt_ecb(&a_ctx, AES_ENCRYPT,nonce_input, nonce);

	/*
	if(file_type == TXT_file)
	{
		write_txt_1array(CETD_nonce, BLK_LENGTH, nonce);
	}
	else
	{
		write_csv_1array(CETD_nonce, BLK_LENGTH, nonce);
	}
	*/
    
	/*
	 compute shift_p and shuffle_p, the length
	 shuffle_p = 2*log2(y_num) + 3*log2(tag_length);
	 shift_p = y_num * log2(tag_length)
	 shift_p_p = log2(tag_length)
	@para s_p:length of a shuffle para
	@para r_p: length of a rotate para
	 */
	int s_p = shuffle_p(y_num, tag_length*CHAR_BIT);	
	int r_p = log2_int(tag_length*CHAR_BIT);

	uchar *s = (uchar *)malloc(sizeof(uchar)*y_num);
	memset(s,0,y_num);
    //shift data
    uchar **shift_data=(uchar **)malloc(sizeof(uchar *)*y_num);
    for(int i=0;i<y_num;i++)
    {
        shift_data[i]=(uchar *)malloc(sizeof(uchar)*tag_length);

        memset(shift_data[i], 0, tag_length);
    }
      
    for(int i=0;i<y_num;i++)
    {
        for(int j=0;j<tag_length;j++)
        {
            *(*(shift_data+i)+j)= CETD_data[i][j];
        }
    }
    
	//prepare s array: rotate para array
    rotate_p(nonce,
		s,
		 y_num,
		 tag_length,
		 r,
		 s_p,
		 r_p);

	/*
 	permutation(shift_data,
		s,
		 y_num, 
		tag_length);
	for(int i=0;i<y_num;i++)
	{
		if(file_type==TXT_file)
		{
			write_txt_1array(y1[i],tag_length,shift_data[i]);
		}	
		else
		{
			write_csv_1array(y1[i],tag_length,shift_data[i]);
		}	

	}
*/
    //the swap_data is the output of 
	//shuffle data
    uchar **swap_data=(uchar **)malloc(sizeof(uchar *)*y_num);
    for(int i=0;i<y_num;i++)
    {
        swap_data[i]=(uchar *)malloc(sizeof(uchar)*tag_length);

        memset(swap_data[i], 0, tag_length);
    }
      
    for(int i=0;i<y_num;i++)
    {
        for(int j=0;j<tag_length;j++)
        {
            *(*(swap_data+i)+j)= CETD_data[i][j];
        }
    }

	/*
	swap(nonce, 
		swap_data, 
		 r, 
		 s_p,
		y_num, tag_length//y_num and tag_len
		);
*/
	/*
	if(file_type==TXT_file){
		write_txt_2array(x,y_num,tag_length,swap_data);
	}
	else
	{
		write_csv_2array(x,y_num,tag_length,swap_data);
	}
	*/

 	permutation(swap_data,
		s,
		 y_num, 
		tag_length);

	/*
    for(int i=0;i<y_num;i++)
	{
		if(file_type==TXT_file)
		{
			write_txt_1array(y2[i],tag_length,swap_data[i]);
		}	
		else
		{
			write_csv_1array(y2[i],tag_length,swap_data[i]);
		}	

	}
*/
	uchar *tag=(uchar *)malloc(sizeof(uchar)*tag_length);
	memset(tag, 0, tag_length);

    tag_gene(swap_data, tag, y_num, tag_length);
	//tag_gene_nonce(swap_data, tag, y_num,tag_length,nonce);
    
	
	if(result_format == DEC)
	{
		write_csv_decimal_1array(CETD_tag,tag_length,tag);
	}
	else
	{
		if(file_type==TXT_file)
		{
			write_txt_1array(CETD_tag,tag_length,tag);
		}
		else
		{
			write_csv_1array(CETD_tag,tag_length,tag);
		}	
	}
	
	
	
	//free memory
   	free(tmp_data);    
	for(int i=0;i<y_num;i++)
	{
		free(CETD_data[i]);
	}
	free(CETD_data);

	free(nonce);
	for(int i=0;i<y_num;i++)
	{
		free(shift_data[i]);
	}
	free(shift_data);

	for(int i=0;i<y_num;i++)
	{
		free(swap_data[i]);
	}
	free(swap_data);
		free(s);

	
	free(tag);

}
