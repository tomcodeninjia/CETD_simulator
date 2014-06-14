#include "../include/shuffle.h"
#include <stdlib.h>
#include <limits.h>
//#include <math.h>
#include <string.h>
#include "../include/math_func.h"
#include "../include/array_shift.h"
#include "../include/show.h"
#define SHUFFLE_BLK_NUM 2
#define ARR_ELEM_LEN CHAR_BIT


/*
 The nonce should be 1 blk
 @para uint* v: each element is the shuffle control for a round
 */

void swap_p(const uchar *nonce, 
		uint *v, 
		int r,
		int shuffle_p)
{
    uint k=0;
    uint blk = 0;
    uint src=0;
	int tmp_v;
    for(int i=0;i<r;i++)
    {
        
		tmp_v = nonce[shuffle_p*(i+1)/CHAR_BIT];
        //k = (*(nonce+shuffle_p*(i+1)/CHAR_BIT)) >> (CHAR_BIT - (shuffle_p*(i+1) % CHAR_BIT));
		k= tmp_v >> (CHAR_BIT - (shuffle_p*(i+1) % CHAR_BIT));
        int dest=shuffle_p*(i+1)/CHAR_BIT;
        blk=dest-src;
        for(int j=0;j<blk;j++)
        {
			tmp_v = nonce[shuffle_p*(i+1)/CHAR_BIT-j-1];
            //k |= (*(nonce + shuffle_p*(i+1)/CHAR_BIT-j-1)) << (((shuffle_p*(i+1))%CHAR_BIT) + j*CHAR_BIT);
			k |= tmp_v <<  (((shuffle_p*(i+1))%CHAR_BIT) + j*CHAR_BIT);
                        
        }
        src=dest;
        v[i] = k & ((1 << shuffle_p)-1);
        
    }
}

//rotate extract bit segments from nonce to form v array
void swap_p_with_start(const uchar *nonce, //
		int start_bit, //0 to 127
		uint *v,//shuffle para array 
		int r,//shuffle rounds
		int shuffle_p // elem_len for v array
		)
{
    uint k=0;
    uint blk = 0;
    uint src=0;
	int tmp_v;
    for(int i=0;i<r;i++)
    {
        
		tmp_v = nonce[shuffle_p*(i+1)/CHAR_BIT];
        //k = (*(nonce+shuffle_p*(i+1)/CHAR_BIT)) >> (CHAR_BIT - (shuffle_p*(i+1) % CHAR_BIT));
		k= tmp_v >> (CHAR_BIT - (shuffle_p*(i+1) % CHAR_BIT));
        int dest=shuffle_p*(i+1)/CHAR_BIT;
        blk=dest-src;
        for(int j=0;j<blk;j++)
        {
			tmp_v = nonce[shuffle_p*(i+1)/CHAR_BIT-j-1];
            //k |= (*(nonce + shuffle_p*(i+1)/CHAR_BIT-j-1)) << (((shuffle_p*(i+1))%CHAR_BIT) + j*CHAR_BIT);
			k |= tmp_v <<  (((shuffle_p*(i+1))%CHAR_BIT) + j*CHAR_BIT);
                        
        }
        src=dest;
        v[i] = k & ((1 << shuffle_p)-1);
        
    }
}

/*
 split the v[i]
 @para log2_int(tag_len)
 */
void v_split(int shuffle_p, int y_num, int tag_length, uchar *v)
{
	int len=0;
	//seg length
	int tmp_len = tag_length * CHAR_BIT;
	*(v+4)= mod1((shuffle_p & ((1 << log2_int(tmp_len)) - 1 )),tmp_len)  ;	
	len += log2_int(tmp_len);
	//blk2 offset
	*(v+3) = mod1(((shuffle_p >> len) & ((1 << log2_int(tmp_len)) - 1 )),tmp_len)  ;
	len += log2_int(tmp_len);
	//blk2 index
	*(v+2) = mod1(((shuffle_p >> len) & ((1 << log2_int(y_num)) - 1)),y_num)  ;
	*(v+2) = y_num-1-*(v+2);
	len += log2_int(y_num);
	//blk1 offset
	*(v+1) =mod1(((shuffle_p >> len) & ((1 << log2_int(tmp_len))-1)),tmp_len)  ; 
	len += log2_int(tmp_len);
	//blk1 index
	*(v+0) =mod1(((shuffle_p >> len) & ((1 << log2_int(y_num))-1)),y_num)  ;
	*(v+0) = y_num-1-*(v+0);

}

void swap(const uchar *nonce,  //nonce 
		uchar **data, //input data array
		int r, //shuffle rounds
		int shuffle_p, //length of v[i]
		int number,  // No. of blocks
		int arr_length// block length
		)
{
	//shuffle para array, 
    uint *v =(uint *)malloc(sizeof(uint)*r);
	memset(v, 0 , r);
    
 	swap_p(nonce, 
		v, 
		 r,
		 shuffle_p);

	//each v[i] is consist of 5 segs, split v[i] to v_s array;
	uchar v_s[5];
	memset(v_s,0,5);
	int tmp_v = 0;

    for(int i=0;i<r;i++)
    {

		memset(v_s,0,5);
		 tmp_v = v[i];
		 //printf("v%x,",v[i]);
		 v_split(tmp_v, number, arr_length, v_s);

		if(v_s[0]==v_s[2])
		{
			v_s[2]  = (v_s[2] + 1) % number;
		}
				
        uchar *a, *b;
        a=data[v_s[0]] ; // index1
        b=data[v_s[2]] ; // index 2


        while(v_s[4]>0 ) //seg_length
        {
            
            if(((*(a+v_s[1] / CHAR_BIT) & (1 << (CHAR_BIT - (v_s[1] % CHAR_BIT)-1))) >> (CHAR_BIT - (v_s[1] % CHAR_BIT)-1)) 
					!= 
					((*(b+v_s[3] / CHAR_BIT) & (1 << (CHAR_BIT - (v_s[3] % CHAR_BIT)-1))) >> (CHAR_BIT - ((v_s[3]) % CHAR_BIT)-1)) )
            {
                
                *(a+v_s[1] / CHAR_BIT) ^= 1 << (CHAR_BIT - ((v_s[1]) % CHAR_BIT)-1);
                *(b+v_s[3] / CHAR_BIT) ^= 1 << (CHAR_BIT - ((v_s[3]) % CHAR_BIT)-1);
                
            }
            
            v_s[1] = mod1((v_s[1] + 1),arr_length*CHAR_BIT );   // offset1
            v_s[3] =mod1((v_s[3] + 1),arr_length*CHAR_BIT); // offset2
			v_s[4]--;
            
        }
    }
    
	free(v);    
}

/* each round replace the segments in two blocks with segments from nonce


*/
void swap_with_nonce(const uchar *nonce, 
		uchar **input, 
		int round, 
		int shuffle_para_len,
		int number, int arr_length//y_num and tag_len
		)
{
	uint *shuffle_para =(uint *)malloc(sizeof(uint)*round);
	memset(shuffle_para, 0 , round);
    
 	swap_p(nonce, 
		shuffle_para, 
		 round,
		 shuffle_para_len);


	//split v[i] to v_s[];
	uchar shuffle_para_arr[5];
	memset(shuffle_para_arr,0,5);
	int tmp_shuffle_para = 0;

	int blk1_offset = BLK_LENGTH*ARR_ELEM_LEN;
	int blk2_offset = BLK_LENGTH*ARR_ELEM_LEN;

    for(int i=0;i<round;i++)
    {

		memset(shuffle_para_arr,0,5);
		tmp_shuffle_para = shuffle_para[i];
		v_split(tmp_shuffle_para, number, arr_length, shuffle_para_arr);

		if(shuffle_para_arr[0]==shuffle_para_arr[2])
		{
			shuffle_para_arr[2]  = (shuffle_para_arr[2] + 1) % number;
		}
		 /*
        struct split *split1;
        split1 = (struct split*)&v[i];
               
        if(split1->index1 == split1->index2)
        {
            split1->index2 = (split1->index2 + 1) % number;
        }
		*/
	    uchar *a, *b;
        a=input[shuffle_para_arr[0]] ; // index1
        b=input[shuffle_para_arr[2]] ; // index2
		//start bits
		
		blk1_offset = blk2_offset-shuffle_para_arr[4];
		if (blk1_offset < 0) blk1_offset += BLK_LENGTH*ARR_ELEM_LEN;
		blk2_offset = blk1_offset-shuffle_para_arr[4];
		if (blk2_offset < 0) blk2_offset += BLK_LENGTH*ARR_ELEM_LEN;

		int tmp_blk1_offset = blk1_offset;
		int tmp_blk2_offset = blk2_offset;

		//search through the segment selected
        while(shuffle_para_arr[4]>0 ) //seg_length
        {
            
			//bit in a is distinct to bit in nonce
            if(((*(a+shuffle_para_arr[1] / ARR_ELEM_LEN) & (1 << (ARR_ELEM_LEN - (shuffle_para_arr[1] % ARR_ELEM_LEN)-1))) >> (ARR_ELEM_LEN - (shuffle_para_arr[1] % ARR_ELEM_LEN)-1)) 
					!= 
					((*(nonce+ tmp_blk1_offset / ARR_ELEM_LEN) & (1 << (ARR_ELEM_LEN - (tmp_blk1_offset % ARR_ELEM_LEN)-1))) >> (ARR_ELEM_LEN - (tmp_blk1_offset % ARR_ELEM_LEN)-1)) )
            {
                
				//revert bit in a
                *(a+shuffle_para_arr[1] / ARR_ELEM_LEN) ^= 1 << (ARR_ELEM_LEN - ((shuffle_para_arr[1]) % ARR_ELEM_LEN)-1);
//                *(b+shuffle_para_arr[3] / CHAR_BIT) ^= 1 << (CHAR_BIT -
//((shuffle_para_arr[3]) % CHAR_BIT)-1);
                
            }

			//bit in a is distinct to bit in nonce
            if(((*(b+shuffle_para_arr[3] / ARR_ELEM_LEN) & (1 << (ARR_ELEM_LEN - (shuffle_para_arr[3] % ARR_ELEM_LEN)-1))) >> (ARR_ELEM_LEN - (shuffle_para_arr[3] % ARR_ELEM_LEN)-1)) 
					!= 
					((*(nonce+ tmp_blk2_offset / ARR_ELEM_LEN) & (1 << (ARR_ELEM_LEN - (tmp_blk2_offset % ARR_ELEM_LEN)-1))) >> (ARR_ELEM_LEN - (tmp_blk2_offset % ARR_ELEM_LEN)-1)) )
            {
                
				//revert bit in a
                *(b+shuffle_para_arr[3] / ARR_ELEM_LEN) ^= 1 << (ARR_ELEM_LEN - ((shuffle_para_arr[3]) % ARR_ELEM_LEN)-1);
//                *(b+shuffle_para_arr[3] / CHAR_BIT) ^= 1 << (CHAR_BIT -
//((shuffle_para_arr[3]) % CHAR_BIT)-1);
                
            }


			//to next bit
            shuffle_para_arr[1] = mod1((shuffle_para_arr[1] + 1),arr_length*ARR_ELEM_LEN);   // offset1
            shuffle_para_arr[3] =mod1((shuffle_para_arr[3] + 1),arr_length*ARR_ELEM_LEN); // offset2
			tmp_blk1_offset = mod1(tmp_blk1_offset+1, BLK_LENGTH*ARR_ELEM_LEN);
			tmp_blk2_offset = mod1(tmp_blk2_offset+1, BLK_LENGTH*ARR_ELEM_LEN);

			shuffle_para_arr[4]--;
            
        }

        
    }
    
	free(shuffle_para);    

}

struct split {
	uint length:5;//segsize
	uint offset2:5;//start position for block2
	uint index2:2;//block number 2
    uint offset1:5;//start position for block1
	uint index1:2;//block number 1
	uint reserved:13;
};

void split_test(uint x)
{
    struct split *split1;
    split1 = (struct split*)&x;
    
    printf("index1 %x, offset1 %x, index2 %x, offset2 %x, length %x\n",split1->index1,split1->offset1,split1->index2,split1->offset2,split1->length);
}

static void print_binary(unsigned int x)
{
	if (x <= 0) {
		return;
	} else {
		print_binary(x / 2);
		printf("%d ", x % 2);
	}
}

void debug(unsigned int x)
{
	print_binary(x);
	printf("\n");
}

