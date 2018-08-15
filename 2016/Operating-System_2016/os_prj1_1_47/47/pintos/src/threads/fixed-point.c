#include "fixed-point.h"

int convert_int_to_fp(int n)
{
    return (n<<14);
}

int convert_fp_to_int(int x)
{
    return (x>>14);
}

int convert_fp_to_int_round(int x)
{
    if(x >= 0)
        return (((x) + (1<<13))>>14);
    else
        return (((x) - (1<<13))>>14);
}

int fp_add(int x, int y)
{
    return x+y;
}

int fp_sub(int x, int y)
{
    return x-y;
}

int add_fp_int(int x,int n)
{
    return x + ((n)<<14);
}

int sub_fp_int(int x,int n)
{
    return x - ((n)<<14);
}

int fp_mul(int x, int y)
{
    return ((((int64_t)x)*y) >> 14);
}

int mul_fp_int(int x,int n)
{
    return x*n;
}

int fp_div(int x,int y)
{
    return (((int64_t)(x)) << 14) / (y);
}

int fp_int_div(int x,int n)
{
    return x/n;
}
