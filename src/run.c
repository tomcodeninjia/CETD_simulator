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
#include <limits.h>

#include "../include/show.h"
#include "../include/input.h"

//CETD API
#include "../include/CETD.h"
#include "../include/aes.h"
#include "../include/cipher.h"
#include "../include/write_txt.h"
#include "../include/write_csv.h"
//AES API

#include <math.h>


#define data_dir_txt "../NIST_suite/long/p%d.txt"
#define ciphertext_dir_txt "../NIST_suite/long/c%d.txt"

#define data_dir_csv "./long/p%d.csv"
#define ciphertext_dir_csv "./long/c%d.csv"

#define CTR_MAX 100000000 
int displayInputOption();
bool displayFileFormat();
int nonce_len(int shuffle_r, int y_num, int tag_len);
int byte_split(int bit_len);

int main()
{
    
	/*
	 Experiment parameters
	 @para test_count: No. of binary sequences
	 @para n: No. of tags for each sequence

	 @para choice: input type
	 @para file_type: file type
	 @para block_number : No. of plaintext blks
	 */
    int test_count;//number of test
    printf("input the No. of sample sequences\n");
    scanf("%d",&test_count);
    
    int n;//number of tags per test
    printf("input the No. of tags per sequence\n");
    scanf("%d",&n);

	int choice =5;
	choice = displayInputOption();
	
	bool file_type = CSV_file;
	file_type = displayFileFormat();

	int block_number;
	printf("Input the No. of Plaintext Blocks.\n");
	scanf("%d",&block_number);

	int block_length;
	printf("Input the Length of Plaintext Blocks(No. of Bytes)\n");
	scanf("%d", &block_length);

	/*
	 CETD parameters
	 @para shuffle_round: shuffle round
	 @para CETD_tag_length: the length of tag

	*/
	int CETD_tag_length;
	printf("Input CETD Tag Length(No. of Bytes, 1-16):\n");
	scanf("%d",&CETD_tag_length);
	while((CETD_tag_length <1)||(CETD_tag_length >16))
	{
		printf("tag length out of order, try again.\n");
		scanf("%d",&CETD_tag_length);
	}

	int shuffle_round;
	printf("Input No. of shuffle rounds:\n");
	scanf("%d",&shuffle_round);

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
	 nonce parameters
	 @para add_len: address length
	 @para crt_len: counter length
	 @para rnd_len: random number length
	 */
	printf("Input nonce parameters\n\n");

	int addr_len;
	printf("Input address length:(bits)\n");
	scanf("%d",&addr_len);

	int crt_len;
	printf("Input counter length:(bits)\n");
	scanf("%d",&crt_len);


	/*
	 * txt files
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
	 * csv files
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

	FILE *fp_nonce_CETD_csv;//tag
    fp_nonce_CETD_csv=NULL;
    char filename_nonce_CETD_csv[256];


	srand((int)time(0));
    
    /**
     Generate input sequences to NIST test, the No. of sequences is test_count
     **/
    for(uint test_round=0;test_round<test_count;test_round++)
    {
        
      
        
        /**
         1 key per sequence,randomly generated
         **/
        uchar *AES_key;
        AES_key = (uchar *)malloc(sizeof(uchar)*16);
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

        
        sprintf(filename_CETD_csv, tag_dir_csv,test_round+1);
        fp_tag_CETD_csv=fopen(filename_CETD_csv, "w");

		sprintf(filename_nonce_CETD_csv, nonce_dir_csv, test_round+1);
		fp_nonce_CETD_csv=fopen(filename_nonce_CETD_csv,"w");

		}
		        
        /**
         Each sequence require several blocks, the No. of blocks is n.
         **/

       		uchar *rnd2;
		   rnd2 = (uchar *)malloc(sizeof(uchar)*block_length);
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
			
			uint counter = test_round;

            //original_data=plaintext
            uchar **original_data;
            original_data=(uchar **)malloc(sizeof(uchar *)*block_number);
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
   
            uchar **ciper_data;
            
            ciper_data=(uchar **)malloc(sizeof(uchar *)*block_number);
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
			memset(rnd_nonce,0,rnd_len);

			for(int i=0;i<rnd_len;i++)
			{
				*(rnd_nonce+i) = rand() %256;
			}

			uchar *CETD_nonce_input;
            CETD_nonce_input = (uchar *)malloc(sizeof(uchar)*16);
			memset(CETD_nonce_input,0,16);

			uint addr = (uint) ciper_data;
			//uint addr = 1;
			uint crt = mod(test_n, CTR_MAX);

 			nonce_input_generation(CETD_nonce_input, 
				 addr,  addr_len,
				 crt,  crt_len,
				rnd_nonce);

			/*
			 *generate CETD_tag
			 * */
			if(file_type == TXT_file)
			{
 				CETD_tag_generation(ciper_data,block_number, 
					block_length,
					CETD_nonce_input, 
					ctx,
					shuffle_round,  //shuffle round  
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
				CETD_tag_generation(ciper_data,block_number, 
					block_length,
					CETD_nonce_input, 
					ctx,
					shuffle_round,  //shuffle round  
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
			for(int i=0;i<block_number;i++)
		{
			fclose(fp_y1_txt[i]);
		}
		for(int i=0;i<block_number;i++)
		{
			fclose(fp_y2_txt[i]);
		}
		fclose(fp_x_blk_txt);
		fclose(fp_tag_CETD_txt);

		fclose(fp_plaintext_txt);
		fclose(fp_cipher_txt);
		fclose(fp_nonce_CETD_txt);

		}
		else
		{
			for(int i=0;i<block_number;i++)
		{
			fclose(fp_y1_csv[i]);
		}
		for(int i=0;i<block_number;i++)
		{
			fclose(fp_y2_csv[i]);
		}
		fclose(fp_x_blk_csv);
		fclose(fp_tag_CETD_csv);

		fclose(fp_plaintext_csv);
		fclose(fp_cipher_csv);

		fclose(fp_nonce_CETD_csv);

		}
		       
    }
     printf("finish \n");
    return 0;
    
}


int displayInputOption()
{
	int choice = 0;
	printf("[1] All 0 Input.\n");
	printf("[2] All 1 Input.\n");
	printf("[3] Balanced Counter Input.\n");
	printf("[4] Normal Linear Counter Input.\n");
	printf("[5] Repeated Random Block Input. \n");
	printf("[6] Random Input.\n");
	printf("\n Please enter the Choice:\n");

	scanf("%d",&choice);

	while((choice <= 0)|| (choice >5))
	{
		printf("error, try again:\n");
		scanf("%d",&choice);
	}
	return choice;

}

bool displayFileFormat()
{
	int choice = 0;
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
