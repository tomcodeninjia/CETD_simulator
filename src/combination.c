#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/combination.h" 
#include "../include/config.h"


int bin2dec(char *bin, int bin_len);
void combinations_bin (int v[], 
		int start, 
		int n, 
		int k, 
		int maxk, 
		char bin[], int bin_len,
		int nCk_arr[], int nCk_arr_len,
		int *nCk_index);



int bin2dec(char *bin, int bin_len)
{
	int i=0;
	int result=0;
	for (i = bin_len-1;i>=0;i--)
	{
		result += bin[i]*pow(2, bin_len-1-i);	
	}
	return result;
}

/*
	@para v[]: 
	@para start:
	@para n:
	@para k:
	@para maxk:
 */
void combinations_bin (int v[], 
		int start, 
		int n, 
		int k, 
		int maxk, 
		char bin[], int bin_len,
		int nCk_arr[], int nCk_arr_len,
		int *nCk_index) 
{
        int     i;
		memset(bin, 0, bin_len);
		int dec=0;

        /* k here counts through positions in the maxk-element v.
         * if k > maxk, then the v is complete and we can use it.
         */
        if (k > maxk) {
                /* insert code here to use combinations as you please */
                for (i=1; i<=maxk; i++) 
				{
//					printf ("%i ", v[i]);
					bin[v[i]-1] = 1;
				}

				dec = bin2dec(bin, bin_len);

				dec = (ELEM_TYPE) dec;
				nCk_arr[*nCk_index] = dec;
				*nCk_index += 1;
                return ;
        }

        /* for this k'th element of the v, try all start..n
         * elements in that position
         */
        for (i=start; i<=n; i++) {

                v[k] = i;

                /* recursively generate combinations of integers
                 * from i+1..n
                 */
                combinations_bin (v, i+1, n, k+1, maxk, bin, bin_len, nCk_arr, nCk_arr_len, nCk_index);
        }
}

