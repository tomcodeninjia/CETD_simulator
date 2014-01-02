#include "../include/tag.h"

void tag_gene(const uchar **data, uchar *tag, int number, int arr_length)
{

    for(int i=0;i<number;i++)
    {
        for(int j=0;j<arr_length;j++)
        {
            
            *(tag+j) = *(tag+j) ^ data[i][j];
        }
    }
}
