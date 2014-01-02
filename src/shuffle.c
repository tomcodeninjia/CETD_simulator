#include "../include/shuffle.h"
#include <stdlib.h>
#include <limits.h>
#include "../include/array_shift.h"
#include "../include/show.h"

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

//split the bits in nonce and store as elements to v, the No. of eles in v=r
//input:nonce,r
//output:v
void shuffle_p(uchar *nonce, uint *v, int r)
{
    uint k=0;
    uint blk = 0;
    uint src=0;
    for(int i=0;i<r;i++)
    {
        
        k = (*(nonce+SHUFFLE_P*(i+1)/CHAR_BIT)) >> (CHAR_BIT - (SHUFFLE_P*(i+1) % CHAR_BIT));
        int dest=SHUFFLE_P*(i+1)/CHAR_BIT;
        //printf(" dest is %d,k is %x \n",dest,k);
        blk=dest-src;
        for(int j=0;j<blk;j++)
        {
            k |= (*(nonce + SHUFFLE_P*(i+1)/CHAR_BIT-j-1)) << (((SHUFFLE_P*(i+1))%CHAR_BIT) + j*CHAR_BIT);
            //printf("des is %d \n",SHUFFLE_P*(i+1)/CHAR_BIT-j-1);
            //int l= (((SHUFFLE_P*(i+1))%CHAR_BIT) + j*CHAR_BIT) ;
            //printf("l is %x \n",l);
            //printf("k is %x\n",k);
            
        }
        src=dest;
        //printf("mask is %x \n",((1 << SHUFFLE_P)-1));
        v[i] = k & ((1 << SHUFFLE_P)-1);
        //printf("v[i] is %x\n",v[i]);
        
    }
}


//nonce is devided according to r， data swapped
void swap(uchar *nonce, uchar **data, int r, int number, int arr_length)
{
    uint *v;
    v=(uint *)malloc(sizeof(uint)*r);
    
    //store parameters
    shuffle_p(nonce, v, r);
    //printf("nonce stored\n");
    for(int i=0;i<r;i++)
    {
        struct split *split1;
        split1 = (struct split*)&v[i];
        //printf("v[i] is %x \n",v[i]);
       // printf("index1 %d, offset1 %d, index2 %d, offset2 %d, length %d\n",split1->index1,split1->offset1,split1->index2,split1->offset2,split1->length);
      // printf("data before swap\n");
      //  for(int i=0;i<BLK_NUMBER;i++)
      // {
      //     show((uchar*)data + i * BLK_LENGTH, BLK_LENGTH);
      // }
        
        uchar *a, *b;
        if(split1->index1 == split1->index2)
        {
            split1->index2 = (split1->index2 + 1) % number;
        }
       // printf("index1 %d, index2 %d\n",split1->index1,split1->index2);
        a=data[split1->index1] ;
        b=data[split1->index2] ;

       // printf("a %d and b %d \n",split1->index1, split1->index2);
      //  show(a, BLK_LENGTH);
      //  show(b, BLK_LENGTH);
        
        
        while(split1->length-- )
        {
            
            if(((*(a+split1->offset1 / CHAR_BIT) & (1 << (CHAR_BIT - (split1->offset1 % CHAR_BIT)-1))) >> (CHAR_BIT - (split1->offset1 % CHAR_BIT)-1)) != ((*(b+split1->offset2 / CHAR_BIT) & (1 << (CHAR_BIT - (split1->offset2 % CHAR_BIT)-1))) >> (CHAR_BIT - ((split1->offset2) % CHAR_BIT)-1)) )
            {
                
                *(a+split1->offset1 / CHAR_BIT) ^= 1 << (CHAR_BIT - ((split1->offset1) % CHAR_BIT)-1);
                *(b+split1->offset2 / CHAR_BIT) ^= 1 << (CHAR_BIT - ((split1->offset2) % CHAR_BIT)-1);
                
            }
            
           // printf("after blk a %x, b %x \n",*(a+split1->offset1 / CHAR_BIT),*(b+split1->offset2 / CHAR_BIT));
            split1->offset1 = (split1->offset1 + 1) % SEG_BITS;
            split1->offset2 = (split1->offset2 + 1) % SEG_BITS;
            //printf("\n");
            
        }
       // printf("After a %d and b %d \n",split1->index1, split1->index2);
      //  show(a, BLK_LENGTH);
      //  show(b, BLK_LENGTH);
       // printf("after swap\n");
       // for(int i=0;i<4;i++)
        //{
         //   show((uchar*)data + i * BLK_LENGTH, BLK_LENGTH);
       // }
         
    }
    
    
    
}


