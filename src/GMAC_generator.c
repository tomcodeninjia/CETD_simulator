//
//  GMAC_generator.c
//  X_generation
//
//  Created by Duke on 2/10/13.
//  Copyright (c) 2013 Duke. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/gcm.h"
#include "../include/GMAC_generator.h"

void gmac_generator(uchar **ciphertext, uchar *iv,int iv_len,uchar *key,int key_len, FILE *gcm_tag, FILE *gcm_output)
{
    
    uchar *c_buffer;
    c_buffer=(uchar *)malloc(sizeof(uchar)*BLK_LENGTH*BLK_NUMBER);
    
    uchar *p1;
    p1=c_buffer;
    int length = 0;
    //printf("input of gmac\n");
    while (length<BLK_LENGTH*BLK_NUMBER) {
        for(int i=0;i<BLK_LENGTH;i++)
        {
            p1[i]=*((uchar*)ciphertext + length +i);
            //printf("%x ",p1[i]);
        }
        length += BLK_LENGTH;
        p1+=BLK_LENGTH;
        //printf("\n");

    }
    //the ciphtext should be stored in c_buffer in order : 0->blk
    {
        //printf("plaintext\n");
        //show((uchar*)original_data + i * BLK_LENGTH, BLK_LENGTH);
        
                //printf("ciphertext\n");
        //show((uchar*)data + i * BLK_LENGTH, BLK_LENGTH);
    }
    aes_context a_ctx;
    int rets=aes_setkey_enc(&a_ctx, key, 128);
    gcm_context ctx;
    unsigned char tag_buf[16];
    int ret;
    uchar *out;
    out=(uchar *)malloc(sizeof(uchar)*BLK_NUMBER*BLK_LENGTH);
    memset(out, 0, BLK_LENGTH*BLK_NUMBER);
    
    gcm_init( &ctx, key, key_len);//the key_len is bit length
    ret = gcm_crypt_and_tag( &ctx, GCM_ENCRYPT,
                            BLK_NUMBER*BLK_LENGTH,c_buffer,//input
                            iv, iv_len,//chosen iv, in application, the iv should be given by simulator
                            16, tag_buf , out);
   length=0;
    uchar *out_p = out;
    while (length < BLK_LENGTH*BLK_NUMBER) {
        //printf("length is %d\n",length);
        for(int i=0;i<BLK_LENGTH;i++)
        {
            fprintf(gcm_output,"%d,",(out_p[i]>>7)&0x01);
            fprintf(gcm_output,"%d,",(out_p[i]>>6)&0x01);
            fprintf(gcm_output,"%d,",(out_p[i]>>5)&0x01);
            fprintf(gcm_output,"%d,",(out_p[i]>>4)&0x01);
            fprintf(gcm_output,"%d,",(out_p[i]>>3)&0x01);
            fprintf(gcm_output,"%d,",(out_p[i]>>2)&0x01);
            fprintf(gcm_output,"%d,",(out_p[i]>>1)&0x01);
            if(length==(BLK_NUMBER-1)*BLK_LENGTH && i==BLK_LENGTH-1)
            {
                fprintf(gcm_output,"%d",out_p[i]&0x01);
            }
            else
            {
                fprintf(gcm_output,"%d,",out_p[i]&0x01);
            }
            
        }
        
        length += BLK_LENGTH;
        out_p += BLK_LENGTH;
    }
    fprintf(gcm_output, "\n");
    
    for(int i=0;i<BLK_LENGTH;i++)
    {
        fprintf(gcm_tag,"%d,",(tag_buf[i]>>7)&0x01);
        fprintf(gcm_tag,"%d,",(tag_buf[i]>>6)&0x01);
        fprintf(gcm_tag,"%d,",(tag_buf[i]>>5)&0x01);
        fprintf(gcm_tag,"%d,",(tag_buf[i]>>4)&0x01);
        fprintf(gcm_tag,"%d,",(tag_buf[i]>>3)&0x01);
        fprintf(gcm_tag,"%d,",(tag_buf[i]>>2)&0x01);
        fprintf(gcm_tag,"%d,",(tag_buf[i]>>1)&0x01);
        if(i==BLK_LENGTH-1)
        {
            fprintf(gcm_tag,"%d",tag_buf[i]&0x01);
        }
        else
        {
            fprintf(gcm_tag,"%d,",tag_buf[i]&0x01);
        }
        
    }
    fprintf(gcm_tag, "\n");

    
}
