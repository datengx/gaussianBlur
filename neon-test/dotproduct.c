#include <stdio.h>
#include <stdlib.h>

#ifdef U0

float
dotprod_fff_cortex_a8(const float *a, const float *b, size_t n)
{
  float sum = 0;
  size_t i;
  for (i = 0; i < n; i++){
    sum += a[i] * b[i];
  }
  return sum;
}

#endif

#ifdef U1
/*
 *  *  preconditions:
 *   *
 *    *    n > 0 and a multiple of 4
 *     *    a   4-byte aligned
 *      *    b  16-byte aligned
 *       */
float
dotprod_fff_cortex_a8(const float *a, const float *b, size_t n)
{
     float s = 0;

    asm volatile ("vmov.f32  q8, #0.0                  \n\t"
         "vmov.f32  q9, #0.0                  \n\t"
         "1:                                  \n\t"
         "subs      %3, %3, #8                \n\t"
         "vld1.32   {d0,d1,d2,d3}, [%1]!      \n\t"
         "vld1.32   {d4,d5,d6,d7}, [%2]!      \n\t"
         "vmla.f32  q8, q0, q2                \n\t"
         "vmla.f32  q9, q1, q3                \n\t"
         "bgt       1b                        \n\t"
         "vadd.f32  q8, q8, q9                \n\t"
         "vpadd.f32 d0, d16, d17              \n\t"
         "vadd.f32  %0, s0, s1                \n\t"
         : "=w"(s), "+r"(a), "+r"(b), "+r"(n)
         :: "q0", "q1", "q2", "q3", "q8", "q9");

    return s;

}
#endif

int main()
{

	volatile float a[256];
	volatile float b[256];
	float dp;
	size_t i;

	for (i=0; i < 256; i++) {
		a[i] = (rand() - RAND_MAX/2.0);
		b[i] = (rand() - RAND_MAX/2.0);
	}

	for (i=0; i<1e6; i++) {
		dp = dotprod_fff_cortex_a8(a, b, 256);
	}

	printf("Result = %f\n", a[0]);

	return 0;
}
