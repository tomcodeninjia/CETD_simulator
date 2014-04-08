#include "../include/shuffle.h"
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include "../include/array_shift.h"
#include "../include/show.h"

int log2_int(int n)
{
	int a= log2(n);
	if( pow(2,a)< n)
	{
		return a+1;
	}
	else 
	{
		return a;
	}
}

int mod1(int a, int b)
{
	return a - (a/b)*b;
}
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

void swap(const uchar *nonce, 
		uchar **data, 
		int r, 
		int shuffle_p,
		int number, int arr_length//y_num and tag_len
		)
{
    uint *v =(uint *)malloc(sizeof(uint)*r);
	memset(v, 0 , r);
    
 	swap_p(nonce, 
		v, 
		 r,
		 shuffle_p);

//    swap_p(nonce, v, r);

	//split v[i] to v_s[];
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
		//printf("v0:%d,v2:%d\n",v_s[0],v_s[2]);
		 /*
        struct split *split1;
        split1 = (struct split*)&v[i];
               
        if(split1->index1 == split1->index2)
        {
            split1->index2 = (split1->index2 + 1) % number;
        }
		*/
				
        uchar *a, *b;
        a=data[v_s[0]] ; // index1
        b=data[v_s[2]] ; // index 2


        while(v_s[4]>0 ) //seg_length
        {
            
//			printf("%d, %d\n", v_s[1],v_s[3]);
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

