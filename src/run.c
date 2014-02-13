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


int displayInputOption();
bool displayFileFormat();
int main()
{
    
	//control paremeter
    int test_count;//number of test
    printf("input the test number\n");
    scanf("%d",&test_count);
    
    int n;//number of tags per test
    printf("input the number of tags\n");
    scanf("%d",&n);

	int choice =6;
	choice = displayInputOption();
	
	
	bool file_type = CSV_file;
	file_type = displayFileFormat();

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
    
    FILE *fp_y1_csv[Y_NUMBER];
	for(int i=0;i<Y_NUMBER;i++)
	{
		fp_y1_csv[i]=NULL;
	}
	char y1_name_csv[Y_NUMBER][256];

	FILE *fp_y2_csv[Y_NUMBER];
	for(int i=0;i<Y_NUMBER;i++)
	{
		fp_y2_csv[i]=NULL;
	}
	char y2_name_csv[Y_NUMBER][256];
    
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
        
		for(int i=0;i<Y_NUMBER;i++)
		{
			sprintf((char*)y1_name_csv + i * 256,y1_dir_split_csv,test_round+1,i+1);
			fp_y1_csv[i]=fopen((char*)y1_name_csv + i * 256,"w");
		}

		for(int i=0;i<Y_NUMBER;i++)
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
		   rnd2 = (uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
		   memset(rnd2,0,BLK_NUMBER);

		   for(int i=0;i<BLK_LENGTH;i++)
		   {
				rnd2[i]=(uchar) (rand()%256);
		   }


        for(uint test_n=0;test_n<n;test_n++)
        {
			int counter = test_round;
			/*
			 * Construct the plaintxt block sequence
			 * */
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


            //original_data=plaintext
            uchar **original_data;
            original_data=(uchar **)malloc(sizeof(uchar *)*BLK_NUMBER);
            for(int i=0;i<BLK_NUMBER;i++)
            {
                original_data[i]=(uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
                
                memset(original_data[i], 0, BLK_LENGTH);
            }
			
			
			if(file_type== TXT_file)
			{
					switch(choice)
			{
				case 1:
					all_0(original_data,fp_plaintext_txt,file_type, BLK_NUMBER, BLK_LENGTH);
					break;
				case 2:
        		 	all_1(original_data, fp_plaintext_txt, file_type, BLK_NUMBER, BLK_LENGTH);
					break;
				case 3:
					linear_counter(original_data,  test_n, file_type, fp_plaintext_txt,BLK_NUMBER , BLK_LENGTH);
					break;
				case 4:
					counter_normal(original_data, counter,file_type, fp_plaintext_txt, BLK_NUMBER, BLK_LENGTH);
				case 5:
					random_repeat_long(original_data, rnd2,fp_plaintext_txt, file_type, BLK_NUMBER, BLK_LENGTH);
					break;
				case 6:
					random_input(original_data, rnd3,fp_plaintext_txt, file_type, BLK_NUMBER, BLK_LENGTH);
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
					all_0(original_data,fp_plaintext_csv,file_type, BLK_NUMBER, BLK_LENGTH);
					break;
				case 2:
        		 	all_1(original_data, fp_plaintext_csv, file_type, BLK_NUMBER, BLK_LENGTH);
					break;
				case 3:
					linear_counter(original_data,  test_n, file_type, fp_plaintext_csv,BLK_NUMBER , BLK_LENGTH);
					break;
				case 4:
					counter_normal(original_data, counter, file_type, fp_plaintext_csv, BLK_NUMBER, BLK_LENGTH);
				case 5:
					random_repeat_long(original_data, rnd2,fp_plaintext_csv, file_type, BLK_NUMBER, BLK_LENGTH);
					break;
				case 6:
					random_input(original_data, rnd3,fp_plaintext_csv, file_type, BLK_NUMBER, BLK_LENGTH);
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
            
            ciper_data=(uchar **)malloc(sizeof(uchar *)*BLK_NUMBER);
            for(int i=0;i<BLK_NUMBER;i++)
            {
                ciper_data[i]=(uchar *)malloc(sizeof(uchar)*BLK_LENGTH);
                
                memset(ciper_data[i], 0, BLK_LENGTH);
            }
            
            
		            //cipher_short(ctx,ciphertext, original_data,fp_cipher_txt,file_type);
            //perodic_cipher(ctx,ciphertext, original_data, fp_cipher_txt,test_n,file_type);

			if(file_type==TXT_file)
			{
		 		only_plaintext(ciper_data, original_data, BLK_NUMBER,BLK_LENGTH, fp_cipher_txt,file_type);
			}
			else
			{
		 		only_plaintext(ciper_data, original_data, BLK_NUMBER,BLK_LENGTH, fp_cipher_csv,file_type);
				
			}

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
            
            typedef struct split_ciphertext {
                uint byte0:8;//segsize
                uint byte1:8;//start position for block2
                uint byte2:8;//block number 2
                uint byte3:8;//start position for block1
            }split_block;

            split_block *s1,*s2;
            s1=(split_block*)&addr_ciphertext;
			s2 =(split_block*)&test_n;
            
            for(int i=8;i<16;i++)
            {
                CETD_nonce_input[i] = (uchar) (rand()%256);
            }
            CETD_nonce_input[0]=s1->byte0;
            CETD_nonce_input[1]=s1->byte1;
            CETD_nonce_input[2]=s1->byte2;
            CETD_nonce_input[3]=s1->byte3;

			CETD_nonce_input[4]=s2->byte0;
            CETD_nonce_input[5]=s2->byte1;
            CETD_nonce_input[6]=s2->byte2;
            CETD_nonce_input[7]=s2->byte3;

			//show(CETD_nonce_input,16);

			
            int r=4;
            int s=0;

			/*
			 *generate CETD_tag
			 * */
			if(file_type == TXT_file)
			{
 				CETD_tag_generation(ciper_data, CETD_nonce_input,  ctx, r, s,  fp_x_blk_txt,fp_y1_txt, fp_y2_txt,fp_tag_CETD_txt, fp_nonce_CETD_txt,file_type);
			}
			else
			{
 				CETD_tag_generation(ciper_data, CETD_nonce_input,  ctx, r, s,  fp_x_blk_csv,fp_y1_csv, fp_y2_csv,fp_tag_CETD_csv,fp_nonce_CETD_csv, file_type);
			}

            
            
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

		if(file_type==TXT_file)
		{
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
		fclose(fp_nonce_CETD_txt);

		}
		else
		{
			for(int i=0;i<BLK_NUMBER;i++)
		{
			fclose(fp_y1_csv[i]);
		}
		for(int i=0;i<BLK_NUMBER;i++)
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

	while((choice <= 0)|| (choice >6))
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
