#include "../include/write_csv.h"
#include <math.h>

void write_csv_2array(FILE *dst, int number, int length, const uchar **array)
{
	for(int i=0;i<number;i++)
    {
        
        for(int j=0;j<length;j++)
        {
			int b = array[i][j];
            fprintf(dst,"%d,",(b>>7)&0x01);
            // printf("%d",((*((uchar*)data + i * length +j))>>7)&0x01);
            fprintf(dst,"%d,",(b>>6)&0x01);
            //printf("%d",((*((uchar*)data + i * length +j))>>6)&0x01);
            fprintf(dst,"%d,",(b>>5)&0x01);
            //printf("%d",((*((uchar*)data + i * length +j))>>5)&0x01);
            fprintf(dst,"%d,",(b>>4)&0x01);
            //printf("%d",((*((uchar*)data + i * length +j))>>4)&0x01);
            fprintf(dst,"%d,",(b>>3)&0x01);
            //printf("%d",((*((uchar*)data + i * length +j))>>3)&0x01);
            fprintf(dst,"%d,",(b>>2)&0x01);
            // printf("%d",((*((uchar*)data + i * length +j))>>2)&0x01);
            fprintf(dst,"%d,",(b>>1)&0x01);
            // printf("%d",((*((uchar*)data + i * length +j))>>1)&0x01);
            
            if((i == number-1)&&(j==length-1))
            {
                fprintf(dst,"%d",b&0x01);
                //printf("%d",(*((uchar*)data + i * length +j))&0x01);
            }
            else
            {
                fprintf(dst,"%d,",b&0x01);
                //printf("%d",(*((uchar*)data + i * length +j))&0x01);
            }
            //printf(" ");
            
        }
        
    }
    fprintf(dst, "\n");
}

 void write_csv_1array(FILE *dst, int length, const uchar *array)
{
	for(int i=0;i<length;i++)
    {
		int b = array[i];
        fprintf(dst,"%d,",(b>>7)&0x01);
        fprintf(dst,"%d,",(b>>6)&0x01);
        fprintf(dst,"%d,",(b>>5)&0x01);
        fprintf(dst,"%d,",(b>>4)&0x01);
        fprintf(dst,"%d,",(b>>3)&0x01);
        fprintf(dst,"%d,",(b>>2)&0x01);
        fprintf(dst,"%d,",(b>>1)&0x01);
        if(i==length-1)
        {
            fprintf(dst,"%d",b&0x01);
        }
        else
        {
            fprintf(dst,"%d,",b&0x01);
        }
        
    }
    fprintf(dst, "\n");

}  

void write_csv_2array2(FILE *dst, int num, int len, const uchar *array)
{
	int length=0;
    uchar *out_p = array;
    while (length < len*num) {
        //printf("length is %d\n",length);
        for(int i=0;i<len;i++)
        {
			int tmp_int=out_p[i];
            fprintf(dst,"%d,",(tmp_int>>7)&0x01);
            fprintf(dst,"%d,",(tmp_int>>6)&0x01);
            fprintf(dst,"%d,",(tmp_int>>5)&0x01);
            fprintf(dst,"%d,",(tmp_int>>4)&0x01);
            fprintf(dst,"%d,",(tmp_int>>3)&0x01);
            fprintf(dst,"%d,",(tmp_int>>2)&0x01);
            fprintf(dst,"%d,",(tmp_int>>1)&0x01);
            if(length==(num-1)*len && i==len-1)
            {
                fprintf(dst,"%d",tmp_int&0x01);
            }
            else
            {
                fprintf(dst,"%d,",tmp_int&0x01);
            }
            
        }
        
        length += len;
        out_p += len;
    }
    fprintf(dst, "\n");
}


void write_csv_decimal_1array(FILE *dst, int len, const uchar *array)
{
	uint tmp_int=0;
	for(int i=0;i<len;i++)
	{
		tmp_int += array[i]*pow(256,i);	
	}

    fprintf(dst,"%d\n",tmp_int);

}

void write_csv_decimal_2array(FILE *dst, int num, int len, const uchar **array)
{


	uint tmp_int=0;	
	if( num * len <= 4)
	{
		for(int i=0;i<num;i++)
		{
			for(int j=0;j<len;j++)
			{
				tmp_int += array[i][j] * pow(256, i*len+j);
			}
		}	
	}

	fprintf(dst,"%d\n", tmp_int);

}
