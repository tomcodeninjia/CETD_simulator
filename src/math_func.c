#include <math.h>
#include "../include/math_func.h"

int log2_int(int n)
{
	int a= log2(n);
	if( pow(2,a)< n)
	{
		return a+1;
	}
	else 
	{
		return a;
	}
}

int mod1(int a, int b)
{
	return a - (a/b)*b;
}

