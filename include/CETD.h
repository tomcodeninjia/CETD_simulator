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

void CETD_tag_generation(const uchar **data, uchar *nonce_input,  aes_context a_ctx,int r, int s,  FILE *x,FILE *y1[Y_NUMBER], FILE *y2[Y_NUMBER], FILE *CETD_tag, bool file_type);
//void CETD_tag_generation(uchar **data, uchar *nonce_input,  aes_context a_ctx,int r, int s, uchar *tag, FILE *x,FILE *y1, FILE *y2[BLK_NUMBER], FILE *CETD_tag, bool file_type);
//void CETD_tag_generation(uchar **data, uchar *nonce_input,  aes_context a_ctx,int r, int s, uchar *tag, FILE *x,FILE *y1,FILE *y2, FILE *CETD_tag, bool file_type);
/*
#define x_dir "data/cetd_test/long/x%d.csv"
#define y1_dir_split_csv "data/cetd_test/short/y1%d-%d.csv"
#define y2_dir_split_csv "data/cetd_test/short/y2%d-%d.csv"
#define tag_dir "data/cetd_test/short/tag_cetd%d.csv"

#define x_dir_txt "data/cetd_test/long/x%d.txt"
#define y1_dir_split_txt "data/cetd_test/short/y1%d-%d.txt"
#define y2_dir_split_txt "data/cetd_test/short/y2%d-%d.txt"
#define tag_dir_txt "data/cetd_test//short/tag_cetd%d.txt"
*/
#define x_dir "../NIST_suite/long/x%d.csv"
#define y1_dir_split_csv "../NIST_suite/short/y1%d-%d.csv"
#define y2_dir_split_csv "../NIST_suite/short/y2%d-%d.csv"
#define tag_dir "../NIST_suite/short/tag_cetd%d.csv"

#define x_dir_txt "../NIST_suite/long/x%d.txt"
#define y1_dir_split_txt "../NIST_suite/short/y1%d-%d.txt"
#define y2_dir_split_txt "../NIST_suite/short/y2%d-%d.txt"
#define tag_dir_txt "../NIST_suite/short/tag_cetd%d.txt"

#endif
