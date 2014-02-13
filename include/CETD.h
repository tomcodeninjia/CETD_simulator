//
//  CETD.h
//  CETD_128
//
//  Created by Duke on 5/03/13.
//  Copyright (c) 2013 Duke. All rights reserved.
//

#ifndef CETD_128_CETD_h
#define CETD_128_CETD_h
#include "shuffle.h"
#include "permutation.h"
#include "tag.h"
#include "aes.h"
#include "config.h"

#define Y_NUMBER BLK_NUMBER*BLK_LENGTH/TAG_LENGTH
//void CETD_tag_generation(uchar **data, uchar *nonce_input,  aes_context a_ctx,int r, int s, uchar *tag, FILE *x,FILE *y1[BLK_NUMBER], FILE *y2[BLK_NUMBER], FILE *CETD_tag, bool file_type);

void CETD_tag_generation(const uchar **data, uchar *nonce_input,  aes_context a_ctx,int r, int s,  FILE *x,FILE *y1[Y_NUMBER], FILE *y2[Y_NUMBER], FILE *CETD_tag, FILE *CETD_nonce,bool file_type);


#define x_dir_txt "../NIST_suite/long/x%d.txt"
#define y1_dir_split_txt "../NIST_suite/short/y1%d-%d.txt"
#define y2_dir_split_txt "../NIST_suite/short/y2%d-%d.txt"
#define tag_dir_txt "../NIST_suite/short/tag_cetd%d.txt"
#define nonce_dir_txt "../NIST_suite/long/nonce_cetd%d.txt"

#define x_dir_csv "./long/x%d.csv"
#define y1_dir_split_csv "./short/y1%d-%d.csv"
#define y2_dir_split_csv "./short/y2%d-%d.csv"
#define tag_dir_csv "./short/tag_cetd%d.csv"
#define nonce_dir_csv "./long/nonce_cetd%d.csv"

#endif
