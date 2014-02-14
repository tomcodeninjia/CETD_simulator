#include "config.h"
//binary output
void write_csv_2array(FILE *dst, int number, int length, const uchar **array);
void write_csv_1array(FILE *dst,  int length, const uchar *array);
void write_csv_2array2(FILE *dst, int num, int len, const uchar *array);

//decimal output
void write_csv_decimal_1array(FILE *dst, int len, const uchar *array);
void write_csv_decimal_2array(FILE *dst, int num, int len, const uchar **array);
