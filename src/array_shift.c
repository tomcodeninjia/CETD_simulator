#include "../include/array_shift.h"


//left shift 1 bit
void shiftl(void *object, size_t size)
{
    unsigned char *byte;
    for ( byte = object; size--; ++byte )
    {
        unsigned char bit = 0;
        if ( size )
        {
            bit = byte[1] & (1 << (CHAR_BIT - 1)) ? 1 : 0;
        }
        *byte <<= 1;
        *byte  |= bit;
    }
}
//right shift n bit
void shiftr(void *object, size_t size, int n)
{
    unsigned char *byte;
    byte=object;
    while(size)
    {
        unsigned char bit =0;
        if(size )
        {
            bit = byte[size-2] & ((1 << n)-1);
        }
        *(byte+size-1) >>= n;
        *(byte+size-1) |= (bit << CHAR_BIT-n);
        size--;
    }
}

//rotate right shift n bit, n<=CHAR_BIT
void shiftrr(void *object, size_t size, int n)
{
    unsigned char *byte;
    //printf("size %d, n %d\n",size,n);
    byte=object;
    unsigned char bit_front =0;
    bit_front = (*(byte+size-1)) & ((1 << n)-1);
    //printf("bit_front %x \n",bit_front);
    while(size)
    {
        
        unsigned char bit =0;
        if(size >= 2)
        {
            bit = (*(byte+size-2)) & ((1 << n)-1);
        }
        else 
        {
            bit = bit_front;
        }
      //  printf("bit is %x\n",bit);
        *(byte+size-1) >>= n;
        *(byte+size-1) |= (bit << CHAR_BIT-n);
        //printf("size %d, byte_end %x \n",size,*(byte+size-1));
        size--;
    }
    *byte |= bit_front << CHAR_BIT-n;
}

//right rotate shift n bits, n < length(array)
void Reverse(char *arr, int b, int e)
{
    for(; b < e; b++, e--)
    {
        char temp = arr[e];
        arr[e] = arr[b];
        arr[b] = temp;
    }
}

void RightShift(char *arr, int N, int k)
{
    k %= N;
    Reverse(arr, 0, N-k-1);
    Reverse(arr, N-k, N-1);
    Reverse(arr, 0, N-1);
}

void shiftrrn(void *object, size_t size, int n)
{
    //printf("before shift\n");
    //show(object, size);
    int m=n/CHAR_BIT;
    RightShift(object, (int)size, m);
   // printf("after block shift\n");
   // show(object, size);
   // printf("shift bit is %d\n",n%CHAR_BIT);
    shiftrr(object, size, n%CHAR_BIT);
   // printf("after bit shift\n");
   // show(object, size);
}
