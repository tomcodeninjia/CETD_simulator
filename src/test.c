#include <stdio.h>
#include <stdlib.h>
#include "../LL_arr_gen.h"

void print_arr(ELEM_TYPE *arr, int len);
void traverse_ll(Node **node_arr, int blk_num);

void main()
{
	//Node array for linkedlists
	Node **node_arr = NULL;
	int blk_num = ELEM_LEN +1;
	node_arr = (Node **)malloc(blk_num*sizeof(Node *));

	LL_arr_gen_func(node_arr, blk_num);

//	traverse_ll(node_arr, blk_num);
	//the linkedlist array is filled and ready to use
	int input_blk_num = 2;
	ELEM_TYPE *input;
	input = (ELEM_TYPE *)malloc(input_blk_num*sizeof(ELEM_TYPE));

	Node **index_tmp;
	index_tmp = (Node **)malloc(input_blk_num*sizeof(Node *));

 	bits_freq_input(input_blk_num, index_tmp, node_arr, input);
	
	//free tmp arrays
	for(int i=0;i<input_blk_num;i++)
	{
		free(index_tmp[i]);	
	}
	free(index_tmp);
	for(int i=0;i<blk_num;i++)
	{
		free(node_arr[i]);	
	}
	free(node_arr);

	free(input);

	exit(0);
}

void print_arr(ELEM_TYPE *arr, int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		printf("%d,",arr[i]);	
	}
	printf("\n");
}

void traverse_ll(Node **node_arr, int blk_num)
{
	for(int i=0;i< blk_num;i++)
	{
		traversal_list(node_arr[i]);		
	}
}


