#include "config.h"
#define OPTV1 0 
#define OPTV2 0 
#define OPTV3 0 
#define OPTV4 1 
#define OPTV5 0

int gf_mult_stage(uchar *nonce, 
		int round_num,
		int shuffle_para_len,
		int shift_para_len,
		uchar **input_data,
		int blk_num,
		int blk_len
		);

int extract_nonce_seg(int start_bit, 
		int end_bit, 
		uchar *nonce, 
		int log_blkn
		);
/*
	@para uchar *a: input array a, blk_len = len_bit/CHAR_BIT 
	@para uchar *b: input array b, blk_len = len_bit/CHAR_BIT 
	@para uchar *output: output array output, blk_len = len_bit/CHAR_BIT 
	@para int len_bit, the length of array in bits
 */
void gf_mult(uchar *input, 
		uchar *nonce_seg, 
		uchar *output, 
		int blk_len, 
		int len_bit
		);
/*
   gf-mult n_blk input blks with segs from nonce
   @para uchar *nonce: nonce array
   @para int round_num: No. of shuffle_round
   @para int shuffle_para_len: 
   @para uchar **input: input[blk_num][blk_len]

 */
int gf_mult_nonce_seg(uchar *nonce, 
		int st_bit,
		int num_blk_mult,
		int round_num,
		int shuffle_para_len,
		uchar **input_data,
		int blk_num,
		int blk_len
		);

