#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/helloworld.h"

void
complex_mult(const float *a, const float *b, float *r) {

	double d0, d1, d2, d3;

	asm volatile (
		"vld1.32	{d0}, [%1]			\n\t"
		"vld1.32	{d1}, [%2]			\n\t"
		"vmul.f32	d2, d0, d1			\n\t"
		"vrev64.32	d3, d0				\n\t"
		"vmul.f32	d3, d3, d1			\n\t"
		"vneg.f32	s5, s5				\n\t"
		"vtrn.32	d2, d3				\n\t"
		"vadd.f32	d2, d2, d3			\n\t"
		"vst1.64	{d2}, [%0]			\n\t"
		:: "r"(r), "r"(a), "r"(b)
		: "d0", "d1", "d2", "d3", "memory");
}

//Time difference function
timespec 
diff(timespec start, timespec end) {
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  This is the main function of the program 
 * =====================================================================================
 */

int 
main ( int argc, char *argv[] ) {

	volatile float a[2], b[2], r[2];
	int i;
	int flag = 0;

	a[0] = 1;
	a[1] = 2;
	b[0] = 3;
	b[1] = 4;

	for (i=0; i<1e8; i++) {
#ifdef NEON
		complex_mult(a, b, r);
#else
		flag = 1;
		r[0] = (a[0]*b[0] - a[1]*b[1]);
		r[1] = (a[1]*b[0] - a[0]*b[1]);
	
#endif
	}
	std::cout << flag << std::endl;
	std::cout << "Result = (" << r[0] << ", " << r[1] << ")" << std::endl;

    timespec time1, time2;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    
        std::cout << "hello world!" << std::endl;
        std::cout << "testing, testing" << std::endl;
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    std::cout << "Hello world message:" << (double)(diff(time1,time2).tv_nsec)/1000000 << " ms\n";

	return 0;
}				/* ----------  end of function main  ---------- */
