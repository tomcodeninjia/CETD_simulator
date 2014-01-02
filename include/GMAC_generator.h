//
//  GMAC_generator.h
//  X_generation
//
//  Created by Duke on 2/10/13.
//  Copyright (c) 2013 Duke. All rights reserved.
//

#ifndef X_generation_GMAC_generator_h
#define X_generation_GMAC_generator_h
#include "aes.h"
#include "gcm.h"
void gmac_generator( uchar **plaintext, uchar *iv,int iv_len,uchar *key, int key_len,FILE *gcm_tag, FILE *gcm_output);
#define gmactag_dir "data/gmac/tag_gmac%d.csv"
#define gmacoutput_dir "data/gmac/mult_gmac%d.csv"
#endif
