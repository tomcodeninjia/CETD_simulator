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

/*
#define data_dir "data/cetd_test/long/p%d.csv"
#define ciphertext_dir "data/cetd_test/long/c%d.csv"
#define nonce_input_dir "data/cetd_test/long/n_i%d.csv"

#define data_dir_txt "data/cetd_test/long/p%d.txt"
#define ciphertext_dir_txt "data/cetd_test/long/c%d.txt"
#define nonce_input_dir_txt "data/cetd_test/long/n_i%d.txt"
*/
#define data_dir "../NIST_suite/long/p%d.csv"
#define ciphertext_dir "../NIST_suite/long/c%d.csv"

#define data_dir_txt "../NIST_suite/long/p%d.txt"
#define ciphertext_dir_txt "../NIST_suite/long/c%d.txt"

//#define iv_dir "/Users/duke/Documents/Research_work/tag_result/Rare_data/scheme_compare/gmac/iv%d.csv"

int main()
{
    
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
    
    FILE *fp_y1_txt[Y_NUMBER];
	for(int i=0;i<Y_NUMBER;i++)
	{
		fp_y1_txt[i]=NULL;
	}
	char y1_name_txt[Y_NUMBER][256];

	FILE *fp_y2_txt[Y_NUMBER];
	for(int i=0;i<Y_NUMBER;i++)
	{
		fp_y2_txt[i]=NULL;
	}
	char y2_name_txt[Y_NUMBER][256];
    
    FILE *fp_tag_CETD_txt;//tag
    fp_tag_CETD_txt=NULL;
    char filename_CETD_txt[256];

	//control paremeter
    int test_count;//number of test
    printf("input the test number\n");
    scanf("%d",&test_count);
    
    int n;//number of tags per test
    printf("input the number of tags\n");
    scanf("%d",&n);
    
    srand((int)time(0));
    
    /**
     Generate input sequences to NIST test, the No. of sequences is test_count
     **/
    for(int test_round=0;test_round<test_count;test_round++)
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
        
		/*
		 *set filenames for txt files
		 * */
		sprintf(filename_plain_txt,data_dir_txt,test_round+1);
		fp_plaintext_txt=fopen(filename_plain_txt,"w");
                
        sprintf(filename_cipher_txt, ciphertext_dir_txt,test_round+1);
        fp_cipher_txt=fopen(filename_cipher_txt, "w");
        
        sprintf(filename_x_txt,x_dir_txt ,test_round+1);
        fp_x_blk_txt=fopen(filename_x_txt, "w");
        
		for(int i=0;i<Y_NUMBER;i++)
		{
			sprintf((char*)y1_name_txt + i * 256,y1_dir_split_txt,test_round+1,i+1);
			fp_y1_txt[i]=fopen((char*)y1_name_txt + i * 256,"w");
		}

		for(int i=0;i<Y_NUMBER;i++)
		{
			sprintf((char*)y2_name_txt + i * 256,y2_dir_split_txt,test_round+1,i+1);
			fp_y2_txt[i]=fopen((char*)y2_name_txt + i * 256,"w");
		}

        
        sprintf(filename_CETD_txt, tag_dir_txt,test_round+1);
        fp_tag_CETD_txt=fopen(filename_CETD_txt, "w");
        
        /**
         Each sequence require several blocks, the No. of blocks is n.
         **/

       		uchar *rnd2;
		   rnd2 = (uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
		   memset(rnd2,0,BLK_NUMBER);

		   for(int i=0;i<BLK_LENGTH;i++)
		   {
				rnd2[i]=(uchar) (rand()%256);
		   }


        for(int test_n=0;test_n<n;test_n++)
        {
			/*
			 * Construct the plaintxt block sequence
			 * */

            //original_data=plaintext
            uchar **original_data;
            original_data=(uchar **)malloc(sizeof(uchar *)*BLK_NUMBER);
            for(int i=0;i<BLK_NUMBER;i++)
            {
                original_data[i]=(uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
                
                memset(original_data[i], 0, BLK_LENGTH);
            }

        //		 all_1(original_data, fp_plaintext_txt, TXT_file, BLK_NUMBER, BLK_LENGTH);
//		all_0(original_data,fp_plaintext_txt,TXT_file, BLK_NUMBER, BLK_LENGTH);

//
 		//linear_counter(original_data,  test_n, TXT_file, fp_plaintext_txt,BLK_NUMBER , BLK_LENGTH);
//			random_repeat_long(original_data, rnd2,fp_plaintext_txt, TXT_file, BLK_NUMBER, BLK_LENGTH);

			uchar **rnd3;
			rnd3=(uchar **)malloc(sizeof(uchar *)*BLK_NUMBER);
            for(int i=0;i<BLK_NUMBER;i++)
            {
                rnd3[i]=(uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
                memset(rnd3[i], 0, BLK_LENGTH);
            }
            
			for(int i=0;i<BLK_NUMBER;i++)
    		{
        		for(int j=0;j<BLK_LENGTH;j++)
        		{
            		*(*(rnd3+i)+j)=(uchar) (rand()%256);
        		}
    		}		

			random_input(original_data, rnd3,fp_plaintext_txt, TXT_file, BLK_NUMBER, BLK_LENGTH);
         	/*
			 *encrypting the plaintext
			 *construct the input block sequence for scheme
			 * */
   
            uchar **ciper_data;
            
            ciper_data=(uchar **)malloc(sizeof(uchar *)*BLK_NUMBER);
            for(int i=0;i<BLK_NUMBER;i++)
            {
                ciper_data[i]=(uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
                
                memset(ciper_data[i], 0, BLK_LENGTH);
            }
            
            
		            //cipher_short(ctx,ciphertext, original_data,fp_cipher_txt,TXT_file);
            //perodic_cipher(ctx,ciphertext, original_data, fp_cipher_txt,test_n,TXT_file);

		 	only_plaintext(ciper_data, original_data, BLK_NUMBER,BLK_LENGTH, fp_cipher_txt,TXT_file);

            /*
             CETD: producing internal outputs and the tag
             */
		     /*
			 *generate cetd_nonce
			 * */
			uint addr_ciphertext = (uint) ciper_data;
           	uchar *CETD_nonce_input;
            CETD_nonce_input = (uchar *)malloc(sizeof(uchar)*16);
			memset(CETD_nonce_input,0,16);
			//void CETD_nonce_generation(FILE *nonce_file,  uchar *nonce_input, int file_type, uint addr_ciphertext)
			            //printf("address is %x\n",addr_ciphertext);
            
            struct split_ciphertext {
                uint byte0:8;//segsize
                uint byte1:8;//start position for block2
                uint byte2:8;//block number 2
                uint byte3:8;//start position for block1
            };
            struct split_ciphertext *s1;
            s1=(struct split_ciphertext*)&addr_ciphertext;
            //printf("byte0 %x,byte1 %x,byte2 %x,byte3 %x\n",s1->byte0,s1->byte1,s1->byte2,s1->byte3);
            
            for(int i=0;i<12;i++)
            {
                CETD_nonce_input[i] = (uchar) (rand()%256);
            }
            CETD_nonce_input[12]=s1->byte0;
            CETD_nonce_input[13]=s1->byte1;
            CETD_nonce_input[14]=s1->byte2;
            CETD_nonce_input[15]=s1->byte3;

			
            int r=4;
            int s=0;

			/*
			 *generate CETD_tag
			 * */
 			CETD_tag_generation(ciper_data, CETD_nonce_input,  ctx, r, s,  fp_x_blk_txt,fp_y1_txt, fp_y2_txt,fp_tag_CETD_txt, TXT_file);

            
            
            free(CETD_nonce_input);

			for(int i=0;i<BLK_NUMBER;i++)
			{
				free(rnd3[i]);
			}
			free(rnd3);

		

        }
        /**
         close the files to prepre for the next input sequence
         **/
        free(rnd2);
        free(AES_key);

		for(int i=0;i<BLK_NUMBER;i++)
		{
			fclose(fp_y1_txt[i]);
		}
		for(int i=0;i<BLK_NUMBER;i++)
		{
			fclose(fp_y2_txt[i]);
		}
		fclose(fp_x_blk_txt);
		fclose(fp_tag_CETD_txt);

		fclose(fp_plaintext_txt);
		fclose(fp_cipher_txt);
       
    }
     printf("finish \n");
    return 0;
    
}


/*
 //gmac outputs
 FILE *fp_gcmoutput;//tag
 fp_gcmoutput=NULL;
 char filename_gcmoutput[256];
 
 FILE *fp_gcmtag;//nonce input
 fp_gcmtag=NULL;
 char filename_gcmtag[256];
 */

/*
 sprintf(filename_gcmoutput, gmacoutput_dir,test_round+1);
 fp_gcmoutput=fopen(filename_gcmoutput, "w");
 
 sprintf(filename_gcmtag, gmactag_dir,test_round+1);
 fp_gcmtag=fopen(filename_gcmtag, "w");
 */

//uchar **gmac_data,*gmac_iv;
/*
 //CETD_data
 gmac_data=(uchar **)malloc(sizeof(uchar *)*BLK_NUMBER);
 for(int i=0;i<BLK_NUMBER;i++)
 {
 gmac_data[i]=(uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
 }
 
 for(int i=0;i<BLK_NUMBER;i++)
 {
 for(int j=0;j<BLK_LENGTH;j++)
 {
 (*((uchar*)gmac_data + i * BLK_LENGTH +j))= (*((uchar*)ciphertext + i * BLK_LENGTH +j));
 }
 }
 
 
 gmac_iv = (uchar *)malloc(sizeof(uchar)*12);
 for(int i=0;i<12;i++)
 {
 gmac_iv[i]=(uchar) (rand()%256);
 }
 gmac_generator( gmac_data, gmac_iv,12,AES_key, 128,fp_gcmtag, fp_gcmoutput);
 */

