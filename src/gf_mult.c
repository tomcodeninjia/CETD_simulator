#include <stdlib.h>
#include <string.h>
#include "../include/gf_mult.h"
#include "../include/math_func.h"
#include "../include/array_shift.h"

#define TESTi 0 
#define DEBUG 1

uchar poly_8[] = {0xD8};
uchar poly_16[] = {0xD4, 0x00};
uchar poly_32[] = {0xB1, 0x00, 0x00, 0x00};
uchar poly_64[] = {0xD8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uchar poly_128[] = {0xE1, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*
   extract a segment from nonce to form a int
   @para start_bit
   @para end_bit
   @para nonce
   @log_blkn
 */
int extract_nonce_seg(int start_bit, int end_bit, uchar *nonce, int log_blkn)
{

	int start_blk = 0;
	start_blk = start_bit / CHAR_BIT;
	int end_blk =0;
	end_blk = end_bit / CHAR_BIT;

	int mult_num = 0; //num of blks doing gf mult	
	mult_num = nonce[start_blk];
	if (start_blk == end_blk)
	{
		mult_num = mult_num >> (CHAR_BIT - 1 - end_bit % CHAR_BIT);
	}
	else
	{
		//assume mult_num is no longer than 8 bits
		mult_num = mult_num << (end_bit % CHAR_BIT +1);

		int tmp = nonce[end_blk];
		mult_num = mult_num | (tmp >> ( CHAR_BIT - 1 - end_bit % CHAR_BIT ));

	}
	mult_num = mult_num & ((1 << log_blkn) - 1);

	return mult_num;

}

/*
	@para uchar *a: input array a, blk_len = len_bit/CHAR_BIT 
	@para uchar *b: input array b, blk_len = len_bit/CHAR_BIT 
	@para uchar *output: output array output, blk_len = len_bit/CHAR_BIT 
	@para int len_bit, the length of array in bits
 */
void gf_mult(uchar *input, 
		uchar *nonce_seg, 
		uchar *output, 
		int blk_len, 
		int len_bit)
{
	uchar tmp_byte;
	memset(output, 0, blk_len);//z=0

	uchar *tmp_v = (uchar *)malloc(blk_len * sizeof(uchar));
	memset(tmp_v, 0, blk_len);

	for(int i=0; i < blk_len; i++)
	{
		*(tmp_v + i) = input[i];	// v=x
	}
	for(int i=0;i < len_bit; i++)	
	{
		tmp_byte = nonce_seg[i/CHAR_BIT] >> (CHAR_BIT-1-(i % CHAR_BIT)); 	//Yi
		tmp_byte = tmp_byte & 0x01;
		if (tmp_byte == 1)
		{
			for(int i=0; i< blk_len; i++)	
			{
				*(output + i) = output[i] ^ tmp_v[i]; // z = z^ v
			}
		}

		tmp_byte = tmp_v[blk_len-1] & 0x1;

		if(tmp_byte == 0)
		{
			shiftr(tmp_v, blk_len, 1);
		}
		else	
		{

			shiftr(tmp_v, blk_len, 1);
			for(int i=0;i < blk_len; i++)
			{
				switch (len_bit) {
					case 8:
						*(tmp_v + i) = tmp_v[i] ^ poly_8[i];	
						break;
					case 16:
						*(tmp_v + i) = tmp_v[i] ^ poly_16[i];	
						break;
					case 32:
						*(tmp_v + i) = tmp_v[i] ^ poly_32[i];	
						break;
					case 64:
						*(tmp_v + i) = tmp_v[i] ^ poly_64[i];	
						break;
					case 128:
						*(tmp_v + i) = tmp_v[i] ^ poly_128[i];	
						break;
				}	
			}

		}

	}
	free(tmp_v);
}

/*
   gf-mult each input blk with 0xFFFF...F or a seg from nonce
   @para uchar *nonce: nonce array
   @para int round_num: No. of shuffle_round
   @para int shuffle_para_len: 
   @para uchar **input: input[blk_num][blk_len]

 */
int gf_mult_stage(uchar *nonce, 
		int round_num,
		int shuffle_para_len,
		int shift_para_len,
		uchar **input_data,
		int blk_num,
		int blk_len
		)
{
	/*
	   extract nonce seg and prepare para array 
	   @para xxx_bit: 0-127
	   @para xxx_blk: 0-15
	 */
	int log_blkn = log2_int(blk_num);

	uchar *mult_index;
	uchar *tmp_mult_input;
	uchar *tmp_mult_result;

	tmp_mult_input = (uchar *)malloc(blk_len * sizeof(uchar));
	memset(tmp_mult_input, 0, blk_len);

	int start_bit =0;
	int end_bit = 0;
	
	/*
	   int mult_num =0;
	   mult_num = extract_nonce_seg(start_bit, end_bit, nonce, log_blkn);

	   memset(mult_index, 0, mult_num);
	   for(int i=0;i<mult_num;i++)
	   {
	//creat a index and array and set index here	
	start_bit = (end_bit + 1) % (BLK_LENGTH * CHAR_BIT);
	end_bit = (start_bit + log_blkn - 1) % (BLK_LENGTH * CHAR_BIT);
	mult_index[i] = extract_nonce_seg(start_bit, end_bit, nonce, log_blkn);
	}
	 */

	start_bit = (round_num * shuffle_para_len + blk_num * shift_para_len) % (BLK_LENGTH * CHAR_BIT);
	end_bit = (start_bit + blk_num -1) % (BLK_LENGTH * CHAR_BIT);
	int mult_mux = 0;
	mult_mux = extract_nonce_seg(start_bit, end_bit, nonce, blk_num);

	mult_index = (uchar *)malloc(blk_num*sizeof(uchar));
	for(int i=0;i<blk_num;i++)
	{
		mult_index[i] = (mult_mux >> i)	& 0x01;
	}

	tmp_mult_result = (uchar *)malloc(blk_len * sizeof(uchar));
	memset(tmp_mult_result, 0, blk_len);

	for(int i=0;i< blk_num;i++)
	{
		//prepare tmp_nonce_mult array
		for(int j=0; j < blk_len; j++)	
		{
#if OPTV1 == 1
			start_bit = (end_bit + 1) % (BLK_LENGTH * CHAR_BIT);
			end_bit = (start_bit + CHAR_BIT - 1) % (BLK_LENGTH * CHAR_BIT);
			tmp_mult_input[j] =   extract_nonce_seg(start_bit, end_bit, nonce, CHAR_BIT);
#endif
#if OPTV2 == 1 
			if(mult_index[i] == 1)
			{
				start_bit = (end_bit + 1) % (BLK_LENGTH * CHAR_BIT);
				end_bit = (start_bit + CHAR_BIT - 1) % (BLK_LENGTH * CHAR_BIT);
				tmp_mult_input[j] =   extract_nonce_seg(start_bit, end_bit, nonce, CHAR_BIT);

			}
			else
			{
				tmp_mult_input[j] = 0xFF;	
			}
#endif
		}
		//mult tmp_nonce_mult with input[i]
		gf_mult(input_data[i], 
				tmp_mult_input, 
				tmp_mult_result, 
				blk_len, 
				blk_len * CHAR_BIT);
		for(int j=0;j< blk_len;j++)
		{
			input_data[i][j] = tmp_mult_result[j];	
		}

		
	}

	free(mult_index);
	free(tmp_mult_input);
	free(tmp_mult_result);
	return end_bit;

}

/*
   gf-mult n_blk input blks with segs from nonce
   @para uchar *nonce: nonce array
   @para int round_num: No. of shuffle_round
   @para int shuffle_para_len: 
   @para uchar **input: input[blk_num][blk_len]

 */
int gf_mult_nonce_seg(uchar *nonce, 
		int st_bit,
		int num_blk_mult,
		int round_num,
		int shuffle_para_len,
		uchar **input_data,
		int blk_num,
		int blk_len
		)
{
	/*
	   extract nonce seg and prepare para array 
	   @para xxx_bit: 0-127
	   @para xxx_blk: 0-15
	 */
	int log_blkn = log2_int(blk_num);

	uchar *mult_index;
	uchar *tmp_mult_input;
	uchar *tmp_mult_result;

	tmp_mult_input = (uchar *)malloc(blk_len * sizeof(uchar));
	memset(tmp_mult_input, 0, blk_len);

	mult_index = (uchar *)malloc(num_blk_mult*sizeof(uchar));
//	int start_bit =0;
//	int end_bit = st_bit - 1;
	//index array of input blks
	
	int start_bit = st_bit;
	int end_bit = (start_bit + log_blkn - 1) % (BLK_LENGTH * CHAR_BIT); 
	mult_index[0] = extract_nonce_seg(start_bit, end_bit, nonce, CHAR_BIT) % blk_num;//which input blk needs gf-mult
	//printf("mult_index 0:%d\n", mult_index[0]);
	if(num_blk_mult != 0)
	{
		for(int i=1;i < num_blk_mult;i++)
		{
			//start_bit = (end_bit + 1) % (BLK_LENGTH * CHAR_BIT);
			//end_bit = (start_bit + log_blkn - 1) % (BLK_LENGTH * CHAR_BIT); 
			mult_index[i] =(mult_index[0] + i) % blk_num ;

		}
			
	}

	tmp_mult_result = (uchar *)malloc(blk_len * sizeof(uchar));
	memset(tmp_mult_result, 0, blk_len);

	for(int i=0;i< num_blk_mult;i++)
	{
		//prepare tmp_nonce_mult array
		for(int j=0; j < blk_len; j++)	
		{
			start_bit = (end_bit + 1) % (BLK_LENGTH * CHAR_BIT);
			end_bit = (start_bit + CHAR_BIT - 1) % (BLK_LENGTH * CHAR_BIT);
			tmp_mult_input[j] =   extract_nonce_seg(start_bit, end_bit, nonce, CHAR_BIT);

		}
		//mult tmp_nonce_mult with input[i]
		gf_mult(input_data[mult_index[i]], 
				tmp_mult_input, 
				tmp_mult_result, 
				 blk_len, 
				 blk_len * CHAR_BIT);
		for(int j=0;j< blk_len;j++)
		{
			input_data[mult_index[i]][j] = tmp_mult_result[j];	
		}
	}

	free(mult_index);
	free(tmp_mult_input);
	free(tmp_mult_result);
	return end_bit;

}

#if TEST == 1
int main()
{
	uchar *nonce = (uchar *)malloc(BLK_LENGTH * sizeof(uchar));
	memset(nonce, 0, BLK_LENGTH);
	for(int i=0;i<BLK_LENGTH;i++)
	{
		nonce[i] = 0xCA;
	}
	while(1)
	{
		printf("input round num:\n");
		int round_num;
		scanf("%d", &round_num);
		printf("input shuffle para len:\n");
		int shuffle_para_len;
		scanf("%d", &shuffle_para_len);
		printf("input blk num:\n");
		int blk_num;
		scanf("%d", &blk_num);
		printf("input blk len:\n");
		int blk_len;
		scanf("%d", &blk_len);

		uchar **input_data;
		input_data = (uchar **)malloc(blk_num * sizeof(uchar *));
		for(int i=0;i < blk_num; i++)
		{
			input_data[i] = (uchar *)malloc(blk_len * sizeof(uchar));
			memset(input_data[i], 0, blk_len);
			for(int j =0; j< blk_len;j++)
			{
				input_data[i][j] = nonce[j];
			}
		}

		gf_mult_stage(nonce, 
				round_num,
				shuffle_para_len,
				input_data,
				blk_num,
				blk_len
				);

		uchar input[1]; 
		memset(input, 0, 1);
		input[0] = 0xCA;
		uchar nonce_seg[1];
		memset(nonce_seg, 0, 1);
		nonce_seg[0] = 0x53;
		uchar output[1];
		gf_mult(input, 
				nonce_seg, 
				output, 
				1, 
				1*CHAR_BIT);
		printf("%d\n",output[0]);
	}
	return 0;

}
#endif

