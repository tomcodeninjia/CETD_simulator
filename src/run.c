//
//  main.c
//  32 bits tag generation
//  CETD, GMAC
//  input data is stored in array
//  Created by Duke on 24/09/12.
//  Copyright (c) 2012 Duke. All rights reserved.
//  better to generate nonce bit by bit
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/config.h"

#include "../include/show.h"
#include "../include/input.h"
#include "../include/cipher.h"
#include "../include/write_txt.h"
#include "../include/write_csv.h"
#include "../include/math_func.h"

//CETD API
#include "../include/CETD.h"

//AES API
#include "../include/aes.h"
//linkedlist APT
#include "../include/LL_arr_gen.h"

#include <math.h>


#define data_dir_txt "../NIST_suite/long/cache%d.txt"
#define ciphertext_dir_txt "../NIST_suite/long/cipher%d.txt"

#define data_dir_csv "./long/cache%d.csv"
#define ciphertext_dir_csv "./long/cipher%d.csv"

#define rndInput_dir_csv "./short/%d_rnd_tag_%d.csv"
#define nonceCollide_tag_dir_csv "./short/%d_tag_cetd_%d.csv"

#define CTR_MAX 100000000 
int displayInputOption();
bool displayFileFormat();
int nonce_len(int shuffle_r, int y_num, int tag_len);
int byte_split(int bit_len);
void traverse_ll(Node **node_arr, int blk_num);

void bits_freq_input(int block_number, 
		int block_length,
		Node **index_tmp, 
		Node **node_arr, 
		ELEM_TYPE **input,
		aes_context ctx,
		int shuffle_round,
		int num_blk_mult,
		char *filename_CETD_csv,
		char *filename_nonce_CETD_csv,
		char *filename_rnd_csv,
		FILE *fp_x_blk_csv,
		FILE **fp_y1_csv, 
		FILE **fp_y2_csv, 
		FILE *fp_tag_CETD_csv, 
		FILE *fp_tag_rnd_csv,
		FILE *fp_nonce_CETD_csv,
		int addr_len,
		int crt_len
		);

//pattern_diff, pattern+no, nos+nos, pas+pas,s-np+s-np
uchar test_array[5][2] = {{0x44, 0x77},{0x66,0x71},{0x34, 0x27},{0x33,0x99},{0x63,0xD8}};

int main(int argc, char *argv[])
{
    
	int choice =6;
//	choice = displayInputOption();
	choice = 4;
	bool file_type = CSV_file;
//	file_type = displayFileFormat();
	file_type = CSV_file;

	/*
	 Experiment parameters
	 @para test_count: No. of binary sequences
	 @para n: No. of tags for each sequence

	 @para choice: input type
	 @para file_type: file type
	 @para block_number : No. of plaintext blks
	 */

	int block_number;
	int block_length;
	int CETD_tag_length;
	int test_count;//number of test
    int n;//number of tags per test

	int shuffle_round;
//	printf("Input No. of shuffle rounds:\n");
//	scanf("%d",&shuffle_round);
	shuffle_round = atof(argv[1]);

	int num_blk_mult;
//	printf("Input No. of GF-mult input blocks:\n");
//	scanf("%d", &num_blk_mult);
	num_blk_mult = atof(argv[2]);

	if(choice == 7)
	{
		block_length =2;	
		block_number =1;
		CETD_tag_length = 1;
	}
	else
	{
//    	printf("input the No. of sample sequences\n");
//    	scanf("%d",&test_count);
//		test_count = 65536;
		test_count = 3;
    	
//    	printf("input the No. of tags per sequence\n");
//    	scanf("%d",&n);
		n = atof(argv[3]);
//		printf("n is %d\n",n);

//		printf("Input the No. of Plaintext Blocks.If choice is (7), input '1':\n");
//		scanf("%d",&block_number);
		block_number = 1;

//		printf("Input the Length of Plaintext Blocks(No. of Bytes). If choice is (7), input '2':\n");
//		scanf("%d", &block_length);
		block_length = atof(argv[4]);

		/*
		 CETD parameters
		 @para shuffle_round: shuffle round
		 @para CETD_tag_length: the length of tag

		*/
//		printf("Input CETD Tag Length(No. of Bytes, 1-16). If choice is (7), input '1':\n");
//		scanf("%d",&CETD_tag_length);
		CETD_tag_length = 1;
		while((CETD_tag_length <1)||(CETD_tag_length >16))
		{
			printf("tag length out of order, try again.\n");
			scanf("%d",&CETD_tag_length);
		}

			
	}

    /*		 
			 compute para based on inputs
	 */
	int y_num;
	if((block_number*block_length) % CETD_tag_length == 0)
	{
		y_num = block_number * block_length / CETD_tag_length;
	}
	else
	{
		y_num = 1 + block_number * block_length / CETD_tag_length;
	}
	// shuffle_round*shuffle_p+shift_p <= block_length

	int n_l = nonce_len(shuffle_round,y_num, CETD_tag_length);
	while(n_l > (block_length * CHAR_BIT))
	{
		printf("nonce length to big, input the following parameter again.\n");
		printf("Input CETD Tag Length(No. of Bytes, 1-16):\n");
		scanf("%d",&CETD_tag_length);
		while((CETD_tag_length <1)||(CETD_tag_length >16))
		{
			printf("tag length out of order, try again.\n");
			scanf("%d",&CETD_tag_length);
			
		}

		printf("Input No. of shuffle rounds:\n");
		scanf("%d",&shuffle_round);

		if((block_number*block_length)% CETD_tag_length == 0)
		{
			y_num = block_number * block_length / CETD_tag_length;
		}
		else
		{
			y_num = 1 + block_number * block_length / CETD_tag_length;
		}
			
		n_l = nonce_len(shuffle_round,y_num, CETD_tag_length);
	}
	//tag_length, y_num and shuffle_r are set now.

	/* 
	 * nonce parameters
	 * @para add_len: address length
	 * @para crt_len: counter length
	 * @para rnd_len: random number length
	 *  
	 */
//	printf("Input nonce parameters\n\n");

	int addr_len;
//	printf("Input address length:(bits)\n");
//	scanf("%d",&addr_len);
	addr_len = 112;

	int crt_len;
//	printf("Input counter length:(bits)\n");
//	scanf("%d",gcrt_len);
	crt_len = 16;


	/*
	 * prepare txt files
	 * */
	FILE *fp_plaintext_txt;
	fp_plaintext_txt=NULL;
	char filename_plain_txt[256];

	FILE *fp_cipher_txt;
	fp_cipher_txt=NULL;
	char filename_cipher_txt[256];
	//
	//cetd outputs
    FILE *fp_x_blk_txt;//only shuffle x
    fp_x_blk_txt=NULL;
    char filename_x_txt[256];
    
    //FILE *fp_y1_txt[y_num];
	FILE **fp_y1_txt = (FILE **)malloc(sizeof(FILE *)*y_num);
	for(int i=0;i<y_num;i++)
	{
		fp_y1_txt[i]=NULL;
	}
	char y1_name_txt[y_num][256];

	FILE **fp_y2_txt = (FILE **)malloc(sizeof(FILE *)*y_num);
	//FILE *fp_y2_txt[y_num];
	for(int i=0;i<y_num;i++)
	{
		fp_y2_txt[i]=NULL;
	}
	char y2_name_txt[y_num][256];
    
    FILE *fp_tag_CETD_txt;//tag
    fp_tag_CETD_txt=NULL;
    char filename_CETD_txt[256];

	FILE *fp_nonce_CETD_txt;//tag
    fp_nonce_CETD_txt=NULL;
    char filename_nonce_CETD_txt[256];

		
	/* 
	 * prepare csv files
	 * */
	FILE *fp_plaintext_csv;
	fp_plaintext_csv=NULL;
	char filename_plain_csv[256];

	FILE *fp_cipher_csv;
	fp_cipher_csv=NULL;
	char filename_cipher_csv[256];
	//
	//cetd outputs
    FILE *fp_x_blk_csv;//only shuffle x
    fp_x_blk_csv=NULL;
    char filename_x_csv[256];
    
    //FILE *fp_y1_csv[y_num];
	FILE **fp_y1_csv = (FILE **)malloc(sizeof(FILE *)*y_num);
	for(int i=0;i<y_num;i++)
	{
		fp_y1_csv[i]=NULL;
	}
	char y1_name_csv[y_num][256];

	//FILE *fp_y2_csv[y_num];
	FILE **fp_y2_csv = (FILE **)malloc(sizeof(FILE *)*y_num);
	for(int i=0;i<y_num;i++)
	{
		fp_y2_csv[i]=NULL;
	}
	char y2_name_csv[y_num][256];
    
    FILE *fp_tag_CETD_csv;//tag
    fp_tag_CETD_csv=NULL;
    char filename_CETD_csv[256];

	FILE *fp_tag_rnd_csv;//tag
    fp_tag_rnd_csv=NULL;
    char filename_rnd_csv[256];

	FILE *fp_nonce_CETD_csv;//tag
    fp_nonce_CETD_csv=NULL;
    char filename_nonce_CETD_csv[256];

	srand((int)time(0));
	if(choice == 7)
	{
		//Node array for linkedlists
		Node **node_arr = NULL;
		int blk_num = ELEM_LEN +1;
		node_arr = (Node **)malloc(blk_num*sizeof(Node *));

		LL_arr_gen_func(node_arr, blk_num);

//		traverse_ll(node_arr, blk_num);
		//the linkedlist array is filled and ready to use

		ELEM_TYPE **input;
		input = (ELEM_TYPE **)malloc(block_number*sizeof(ELEM_TYPE*));
		for(int i=0;i<block_number;i++)
		{
			input[i] = (ELEM_TYPE *)malloc(block_length*sizeof(ELEM_TYPE));
		}
		
		int input_blk_num = block_length*block_number;
		Node **index_tmp;
		index_tmp = (Node **)malloc(input_blk_num*sizeof(Node *));

		//aes key
		uchar *AES_key;
    	AES_key = (uchar *)malloc(sizeof(uchar)*16);
    	memset(AES_key, 0, 16);
    	    
    	for(int i=0;i<16;i++)
    	{
    	    *(AES_key+i)= (uchar) (rand()%256);
    	}

    	aes_context ctx;
    	aes_setkey_enc(&ctx, AES_key, 128);

     	bits_freq_input( block_number, 
					block_length,
					index_tmp, 
					node_arr, 
					input,
					ctx,
					shuffle_round,
					num_blk_mult,
					filename_CETD_csv,
					filename_nonce_CETD_csv,
					filename_rnd_csv,
					fp_x_blk_csv,
					fp_y1_csv, 
					fp_y2_csv, 
					fp_tag_CETD_csv, 
					fp_tag_rnd_csv,
					fp_nonce_CETD_csv,
					addr_len,
					crt_len
					);


				
		//free tmp arrays
		for(int i=0;i<input_blk_num;i++)
		{
			free(index_tmp[i]);	
		}
		free(index_tmp);
		for(int i=0;i<blk_num;i++)
		{
			free(node_arr[i]);	
		}
		free(node_arr);

		for(int i=0;i<block_number;i++)
		{
			free(input[i]);
		}
		free(input);
		free(AES_key);
	
	}
	else{
		/**
     	Generate input sequences to NIST test, the No. of sequences is test_count
     	**/
		for(uint test_round=0;test_round<test_count;test_round++)
    	{
    	     /**
    	    	aes_context ctx generation 
				1 ctx for each sequence
    	     **/
    	    uchar *AES_key;
    	    AES_key = (uchar *)malloc(sizeof(uchar)*16);
			uchar *rnd2;
			rnd2 = (uchar *)malloc(sizeof(uchar)*block_length);

    	    memset(AES_key, 0, 16);
    	    //show(AES_key, 16);
    	    
    	    for(int i=0;i<16;i++)
    	    {
    	        *(AES_key+i)= (uchar) (rand()%256);
    	    }
			//
    	    //set aes key for all the block cipehr

    	    aes_context ctx;
    	    aes_setkey_enc(&ctx, AES_key, 128);
    	    
			if(file_type==TXT_file)
			{
				/*
				 *set filenames for txt files
				 * */

				/*
				sprintf(filename_plain_txt,data_dir_txt,test_round+1);
				fp_plaintext_txt=fopen(filename_plain_txt,"w");
    	    	        
    	    	sprintf(filename_cipher_txt, ciphertext_dir_txt,test_round+1);
    	    	fp_cipher_txt=fopen(filename_cipher_txt, "w");
    	    	
    	    	sprintf(filename_x_txt,x_dir_txt ,test_round+1);
    	    	fp_x_blk_txt=fopen(filename_x_txt, "w");
    	    	
				for(int i=0;i<y_num;i++)
				{
					sprintf((char*)y1_name_txt + i * 256,y1_dir_split_txt,test_round+1,i+1);
					fp_y1_txt[i]=fopen((char*)y1_name_txt + i * 256,"w");
				}

				for(int i=0;i<y_num;i++)
				{
					sprintf((char*)y2_name_txt + i * 256,y2_dir_split_txt,test_round+1,i+1);
					fp_y2_txt[i]=fopen((char*)y2_name_txt + i * 256,"w");
				}
    	    	
				*/
    	    	sprintf(filename_CETD_txt, tag_dir_txt,test_round+1);
    	    	fp_tag_CETD_txt=fopen(filename_CETD_txt, "w");

				sprintf(filename_nonce_CETD_txt, nonce_dir_txt, test_round+1);
				fp_nonce_CETD_txt=fopen(filename_nonce_CETD_txt,"w");
					
			}
			else
			{
				/*
				 *set filenames for csv files
				 * */

				/*
				sprintf(filename_plain_csv,data_dir_csv,test_round+1);
				fp_plaintext_csv=fopen(filename_plain_csv,"w");
		                
		        sprintf(filename_cipher_csv, ciphertext_dir_csv,test_round+1);
		        fp_cipher_csv=fopen(filename_cipher_csv, "w");
		        
		        sprintf(filename_x_csv,x_dir_csv ,test_round+1);
		        fp_x_blk_csv=fopen(filename_x_csv, "w");
		        
				for(int i=0;i<y_num;i++)
				{
					sprintf((char*)y1_name_csv + i * 256,y1_dir_split_csv,test_round+1,i+1);
					fp_y1_csv[i]=fopen((char*)y1_name_csv + i * 256,"w");
				}
		
				for(int i=0;i<y_num;i++)
				{
					sprintf((char*)y2_name_csv + i * 256,y2_dir_split_csv,test_round+1,i+1);
					fp_y2_csv[i]=fopen((char*)y2_name_csv + i * 256,"w");
				}
				*/
		        sprintf(filename_CETD_csv, tag_dir_csv,test_round+1);
		        fp_tag_CETD_csv=fopen(filename_CETD_csv, "w");
		
				sprintf(filename_nonce_CETD_csv, nonce_dir_csv, test_round+1);
				fp_nonce_CETD_csv=fopen(filename_nonce_CETD_csv,"w");
		
			}
			        
    	    /**
    	     Each sequence require several blocks, the No. of blocks is n.
    	     **/

    	   	memset(rnd2,0,block_number);
			for(int i=0;i<block_length;i++)
			{
				rnd2[i]=(uchar) (rand()%256);
			}


    	    for(uint test_n=0;test_n<n;test_n++)
    	    {
				/*
				 * Construct the plaintxt block sequence
				 * */
				uchar **rnd3;
				rnd3=(uchar **)malloc(sizeof(uchar *)*block_number);
				uchar **original_data;
    	        original_data=(uchar **)malloc(sizeof(uchar *)*block_number);
				uchar **ciper_data;
    	        ciper_data=(uchar **)malloc(sizeof(uchar *)*block_number);

    	        for(int i=0;i<block_number;i++)
    	        {
    	            rnd3[i]=(uchar *)malloc(sizeof(uchar)*block_length);
    	            memset(rnd3[i], 0, block_length);
    	        }
    	        
				for(int i=0;i<block_number;i++)
    			{
    	    		for(int j=0;j<block_length;j++)
    	    		{
    	        		*(*(rnd3+i)+j)=(uchar) (rand()%256);
    	    		}
    			}		
				
				//test_round: No. of sequences
				uint counter = test_round;

    	        //original_data=plaintext
    	        for(int i=0;i<block_number;i++)
    	        {
    	            original_data[i]=(uchar *)malloc(sizeof(uchar)*block_length);
    	            
    	            memset(original_data[i], 0, block_length);
    	        }
				
				if(file_type== TXT_file)
				{
						switch(choice)
				{
					case 1:
						all_0(original_data,fp_plaintext_txt,file_type, block_number, block_length);
						break;
					case 2:
    	    		 	all_1(original_data, fp_plaintext_txt, file_type, block_number, block_length);
						break;
					case 3:
						linear_counter(original_data,  test_n, file_type, fp_plaintext_txt,block_number , block_length);
						break;
					case 4:
 						counter_normal(original_data,counter, fp_plaintext_txt, file_type, block_number, block_length);
						break;
					case 5:
						random_repeat_long(original_data, rnd2,fp_plaintext_txt, file_type, block_number, block_length);
						break;
					case 6:
						//void random_input(uchar **input, const uchar **rnd,FILE *data_i, bool file_type, int number, int arr_length)

						random_input(original_data, rnd3,fp_plaintext_txt, file_type, block_number, block_length);
						break;
					default:
						printf("Error: Please Try again.\n");
						break;
				}
				
				}
				else
				{
					switch(choice)
				{
					case 1:
						all_0(original_data,fp_plaintext_csv,file_type, block_number, block_length);
						break;
					case 2:
    	    		 	all_1(original_data, fp_plaintext_csv, file_type, block_number, block_length);
						break;
					case 3:
						linear_counter(original_data,  test_n, file_type, fp_plaintext_csv,block_number , block_length);
						break;
					case 4:
 						counter_normal(original_data,counter, fp_plaintext_csv, file_type, block_number, block_length);
						break;
					case 5:
						random_repeat_long(original_data, rnd2,fp_plaintext_csv, file_type, block_number, block_length);
						break;
					case 6:
						random_input(original_data, rnd3,fp_plaintext_csv, file_type, block_number, block_length);
						break;
					default:
						printf("Error: Please Try again.\n");
						break;
				}

				}

    	     	/*
				 *encrypting the plaintext
				 *construct the input block sequence for scheme
				 * */
   
    	        for(int i=0;i<block_number;i++)
    	        {
    	            ciper_data[i]=(uchar *)malloc(sizeof(uchar)*block_length);
    	            
    	            memset(ciper_data[i], 0, block_length);
    	        }
    	        
    	        
			    //cipher_short(ctx,ciphertext, original_data,fp_cipher_txt,file_type);
    	        //perodic_cipher(ctx,ciphertext, original_data, fp_cipher_txt,test_n,file_type);

				if(file_type==TXT_file)
				{
			 		only_plaintext(ciper_data, original_data, block_number,block_length, fp_cipher_txt,file_type);
				}
				else
				{
			 		only_plaintext(ciper_data, original_data, block_number,block_length, fp_cipher_csv,file_type);
					
				}

				int CETD_ctr = mod(test_n,CTR_MAX);
    	        /*
    	         CETD: producing internal outputs and the tag
    	         */
			     /*
				 *generate cetd_nonce
				 @para add_length
				 @para rnd_length
				 @para cnt_length
				 * */
				/*
				 nonce parameter
				 @para int addr_cipher
				 @para uchar *rnd
				 @para uchar 
				 */
				int rnd_len = byte_split(128 - addr_len - crt_len) ;
				uchar *rnd_nonce = (uchar *)malloc(sizeof(uchar)*rnd_len);
				uchar *CETD_nonce_input;
    	        CETD_nonce_input = (uchar *)malloc(sizeof(uchar)*16);

				memset(rnd_nonce,0,rnd_len);
				for(int i=0;i<rnd_len;i++)
				{
					*(rnd_nonce+i) = rand() %256;
				}

				memset(CETD_nonce_input,0,16);

				uint addr = (uint) ciper_data;
				 addr = 1;
				uint crt = mod(test_n, CTR_MAX);

 				nonce_input_generation(CETD_nonce_input, 
					 addr,  addr_len,
					 crt,  crt_len,
					rnd_nonce);

				/*
				for(int i=0;i<block_number;i++)
				{
					for(int j=0;j<block_length;j++)
					{
						*(*(ciper_data+i)+j) = test_array[test_round][j];
					}
				}
				*/
				/*
				 *generate CETD_tag
				 * */
				if(file_type == TXT_file)
				{
					CETD_tag_generation(ciper_data,
							block_number, 
							block_length,
							CETD_nonce_input, 
							ctx,
							shuffle_round,  //shuffle round  
							num_blk_mult,
							CETD_tag_length,
							y_num, //
							fp_x_blk_txt,
							fp_y1_txt, 
							fp_y2_txt, 
							fp_tag_CETD_txt, 
							fp_nonce_CETD_txt,
							file_type,
							DEC);

				}
				else
				{
					CETD_tag_generation(ciper_data,
							block_number, 
							block_length,
							CETD_nonce_input, 
							ctx,
							shuffle_round,  //shuffle round  
							num_blk_mult,
							CETD_tag_length,
							y_num, //
							fp_x_blk_csv,
							fp_y1_csv, 
							fp_y2_csv, 
							fp_tag_CETD_csv, 
							fp_nonce_CETD_csv,
							file_type,
							DEC);

				}
    	        
    	        free(CETD_nonce_input);
				free(rnd_nonce);

				for(int i=0;i<block_number;i++)
				{
					free(rnd3[i]);
				}
				free(rnd3);
				
				for(int i=0;i<block_number;i++)
				{
					free(ciper_data[i]);
				}
				free(ciper_data);
				
				for(int i=0;i<block_number;i++)
				{
					free(original_data[i]);
				}
				free(original_data);
    	    }
    	    /**
    	     close the files to prepre for the next input sequence
    	     **/
    	    free(rnd2);
    	    free(AES_key);

			if(file_type==TXT_file)
			{
				/*
				fclose(fp_plaintext_txt);
				fclose(fp_cipher_txt);
				for(int i=0;i<block_number;i++)
				{
					fclose(fp_y1_txt[i]);
				}
				fclose(fp_x_blk_txt);
				for(int i=0;i<block_number;i++)
				{
					fclose(fp_y2_txt[i]);
				}
				*/
							
				fclose(fp_nonce_CETD_txt);
				fclose(fp_tag_CETD_txt);
			}
			else
			{
				/*
				fclose(fp_plaintext_csv);
				fclose(fp_cipher_csv);

				for(int i=0;i<block_number;i++)
				{
					fclose(fp_y1_csv[i]);
				}
				fclose(fp_x_blk_csv);
				for(int i=0;i<block_number;i++)
				{
					fclose(fp_y2_csv[i]);
				}
				*/
			
				fclose(fp_nonce_CETD_csv);
				fclose(fp_tag_CETD_csv);

			}
			       
    	}
	
	}
    
    printf("finish \n");
   	return 0; 
}


int displayInputOption()
{
	int choice = 0;
	printf("choose input type:\n");
	printf("[1] All 0 Input.\n");
	printf("[2] All 1 Input.\n");
	printf("[3] Balanced Counter Input.\n");
	printf("[4] Normal Linear Counter Input.\n");
	printf("[5] Repeated Random Block Input. \n");
	printf("[6] Random Input.\n");
	printf("[7] Bit Frequency Input.\n");
	printf("\n Please enter the Choice:\n");

	scanf("%d",&choice);

	while((choice <= 0)|| (choice >7))
	{
		printf("error, try again:\n");
		scanf("%d",&choice);
	}
	return choice;

}

bool displayFileFormat()
{
	int choice = 0;
	printf("choose file type:\n");
	printf("[1] TXT format: raw data stored in ../NIST_suite/long and /short\n");
	printf("[2] CSV format: raw data stored in ./long and ./short\n");

	scanf("%d",&choice);
	while((choice<=0)|| (choice >2))
	{
		printf("Error input, try again.\n");
		scanf("%d",&choice);
	}

	switch(choice)
	{
		case 1:
			return TXT_file;
		case 2:
			return CSV_file;
		default:
			return CSV_file;
	}

}

int nonce_len(int shuffle_r, int y_num, int tag_len)
{
	int v, s;
	
	v= shuffle_p(y_num, tag_len); 
	s= log2_int(tag_len);

	return shuffle_r*v+y_num*s;

}

int byte_split(int bit_len)
{
	if(mod(bit_len, CHAR_BIT) == 0)
	{
		return bit_len/CHAR_BIT;
	}
	else 
	{
		return (bit_len/CHAR_BIT) +1;
	}
}
void traverse_ll(Node **node_arr, int blk_num)
{
	for(int i=0;i< blk_num;i++)
	{
		traversal_list(node_arr[i]);		
	}
}

void bits_freq_input(int block_number, 
		int block_length,
		Node **index_tmp, 
		Node **node_arr, 
		ELEM_TYPE **input,
		aes_context ctx,
		int shuffle_round,
		int num_blk_mult,
		char *filename_CETD_csv,
		char *filename_nonce_CETD_csv,
		char *filename_rnd_csv,
		FILE *fp_x_blk_csv,
		FILE **fp_y1_csv, 
		FILE **fp_y2_csv, 
		FILE *fp_tag_CETD_csv, 
		FILE *fp_tag_rnd_csv,
		FILE *fp_nonce_CETD_csv,
		int addr_len,
		int crt_len
		)
{

	srand((int)time(0));
	int input_blk_num = block_length*block_number;

	int rnd_len = byte_split(128 - addr_len - crt_len) ;
	uchar *rnd_nonce = (uchar *)malloc(sizeof(uchar)*rnd_len);

	uchar *CETD_nonce_input;
   	CETD_nonce_input = (uchar *)malloc(sizeof(uchar)*16);

	uchar **rnd_input = (uchar **)malloc(block_number*sizeof(uchar *));
	for(int i=0;i<block_number;i++)
	{
		rnd_input[i] = (uchar *)malloc(block_length*sizeof(uchar));
	}

	int nonce_num = 0;
	printf("input No. of nonces\n");
	scanf("%d",&nonce_num);
	if(input_blk_num == 2)
	{
		for(int x =0; x < nonce_num; x++)
		{
			// there is i 1s in the input
			for(int i=0;i<= input_blk_num*ELEM_LEN;i++)
			{
				int count =0;
				//there are i 1s
				//all input arrays stored in a file?
				sprintf(filename_CETD_csv, nonceCollide_tag_dir_csv,x,i);
				fp_tag_CETD_csv=fopen(filename_CETD_csv, "w");

				sprintf(filename_nonce_CETD_csv, nonce_dir_csv, i);
				fp_nonce_CETD_csv=fopen(filename_nonce_CETD_csv,"w");

				sprintf(filename_rnd_csv,rndInput_dir_csv,x,i);
				fp_tag_rnd_csv=fopen(filename_rnd_csv, "w");

				//open a file here
				//fp_tag_cetd = fopen()
				//fp_nonce = fopen()
				memset(rnd_nonce,0,rnd_len);

				for(int i=0;i<rnd_len;i++)
				{
					*(rnd_nonce+i) = rand() %256;
				}

				memset(CETD_nonce_input,0,16);

				uint addr = (uint) input;
				//uint addr = 1;
				uint crt = i;

				nonce_input_generation(CETD_nonce_input, 
						addr,  addr_len,
						crt,  crt_len,
						rnd_nonce);

				// nonce_input = ....
				//gen a nonce here
				int start = i>ELEM_LEN?i-ELEM_LEN:0;	
				int end = i>ELEM_LEN?ELEM_LEN:i;

				for(int j=start;j<=end;j++)	
				{
					//first blk has j 1s
					index_tmp[0] = node_arr[j];
					while(index_tmp[0] != NULL)
					{
						//second blk has i-j 1s
						index_tmp[1] = node_arr[i-j];
						input[0][0] = index_tmp[0]->data;	
						index_tmp[0] = index_tmp[0]->next;
						while(index_tmp[1] != NULL)
						{
							input[0][1] = index_tmp[1]->data; 		
							index_tmp[1] = index_tmp[1]->next;
							count++;
							//gen a tag here and write to the file
							/*
							   generate input, gen tag then write to a line in fp_tag 
							   */
							for(int i=0;i<block_number;i++)	
							{
								memset(rnd_input[i], 0 , block_length);
							}

							for(int i=0;i<block_number;i++)
							{
								for(int j=0;j<block_length;j++)	
								{
									rnd_input[i][j] = (uchar) (rand()%256);
								}
							}
							CETD_tag_generation(input,
									block_number, 
									block_length,
									CETD_nonce_input, //uchar *
									ctx, //aes_context
									shuffle_round,  //shuffle round  
									num_blk_mult,
									1,
									2, //
									fp_x_blk_csv,
									fp_y1_csv, 
									fp_y2_csv, 
									fp_tag_CETD_csv, 
									fp_nonce_CETD_csv,
									CSV_file,
									DEC);
							CETD_tag_generation(rnd_input,
									block_number, 
									block_length,
									CETD_nonce_input, //uchar *
									ctx, //aes_context
									shuffle_round,  //shuffle round  
									num_blk_mult,
									1,
									2, //
									fp_x_blk_csv,
									fp_y1_csv, 
									fp_y2_csv, 
									fp_tag_rnd_csv, 
									fp_nonce_CETD_csv,
									CSV_file,
									DEC);
							//use input to tag generation 
						}
					}
				}
//				printf("NO. of %d 1s array is:%d\n", i, count);
				fclose(fp_tag_rnd_csv);
				fclose(fp_tag_CETD_csv);
				fclose(fp_nonce_CETD_csv);
			}


		}
	}

	free(rnd_nonce);
	free(CETD_nonce_input);
	for(int i=0;i<block_number;i++)
	{
		free(rnd_input[i]);
	}
	free(rnd_input);
}
