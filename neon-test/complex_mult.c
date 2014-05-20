#include <stdio.h>
#include <stdlib.h>

void
complex_mult(const float *a, const float *b, float *r)
{

    double d0, d1, d2, d3;

#if  0
    asm volatile (
         "vld1.32   {%P0}, [%5]                \n\t"
         "vld1.32   {%P1}, [%6]                \n\t"
         "vmul.f32  %P2, %P0, %P1              \n\t"
         "vrev64.32 %P3, %P0                   \n\t"
         "vmul.f32  %P3, %P3, %P1              \n\t"
         "vneg.f32  %2,  %2                    \n\t"
         "vtrn.32   %P2, %P3                   \n\t"
         "vsub.f32  %P2, %P2, %P3              \n\t"
         "vst1.64   {%P2}, [%4]                \n\t"
         : "=&w"(d0), "=&w"(d1), "=&w"(d2), "=&w"(d3)
         : "r"(r), "r"(a), "r"(b)
         : "memory");
#else
    asm volatile (
         "vld1.32   {d0}, [%1]                \n\t"
         "vld1.32   {d1}, [%2]                \n\t"
         "vmul.f32  d2, d0, d1                \n\t"
         "vrev64.32 d3, d0                    \n\t"
         "vmul.f32  d3, d3, d1                \n\t"
         "vneg.f32  s5, s5                    \n\t"
         "vtrn.32   d2, d3                    \n\t"
         "vadd.f32  d2, d2, d3                \n\t"
         "vst1.64   {d2}, [%0]                \n\t"
         :: "r"(r), "r"(a), "r"(b)
         : "d0", "d1", "d2", "d3", "memory");
#endif
}


int main()
{

	volatile float a[2], b[2], r[2];

	int i;

	a[0] = 1;
	a[1] = 2;
	b[0] = 3;
	b[1] = 4;

	for (i=0; i<1e8; i++) {
#ifdef NEON
		complex_mult(a, b, r);
#else
		r[0] = (a[0]*b[0] - a[1]*b[1]);
		r[1] = (a[1]*b[0] + a[0]*b[1]);
#endif
	}
	printf("Result = (%f, %f)\n", r[0], r[1]);

	return 0;
}
