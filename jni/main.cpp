#include <iostream>
#include <time.h>

#include "include/helloworld.h"
#include "include/convolution.h"

void
complex_mult(const float *a, const float *b, float *r) {

	double d0, d1, d2, d3;

	asm volatile (
		"vld1.32	{d0}, [%1]			\n\t"
		"vld1.32	{d1}, [%2]			\n\t"
		"vadd.f32	d2, d0, d1			\n\t"
		"vst1.64	{d2}, [%0]			\n\t"
		:: "r"(r), "r"(a), "r"(b)
		: "d0", "d1", "d2", "d3", "memory");
}

void
vec_addition(const float *a, const float *b, float *r, float *r1) {
	double d0, d1, d2, d3;

	asm volatile (
		"vldmia.32	%1, {d3}			\n\t"	// ld 1st op in d3
		"vld1.32	{d0}, [%1]			\n\t"	// ld 1st op in d0
		"vld1.32	{d1}, [%2]			\n\t"	// ld 2nd op in d1
		"vadd.f32	d2, d0, d1			\n\t"	// do addition
		"vadd.f32	d4, d3, d1			\n\t"
		
		"vst1.64	{d2}, [%0]			\n\t"	// st in d2
		"vst1.64	{d4}, [%3]			\n\t"	// st in d4
		

		:: "r"(r), "r"(a), "r"(b), "r"(r1)
		:	"d0", "d1", "d2", "d3", "d4", "memory");
}

void 
complex_abs(const float *a, const float *b, float *r) {

	asm volatile (
		""
		);


}

/** @fn 3x3 matrix determinant
*	3x3 matrix determinant using neon
*/
float 
matrix_determinant_3x3_neon(const float *matrix) {
	float det = 0.0;

	asm volatile (
		// load the first column with 0
		"vmov.f32	d0, #0.0				\n\t"	// Move: d0 = 0.0
		"vmov.f32	d2, #0.0				\n\t"	// Move: d2 = 0.0
		"vmov.f32	d4, #0.0				\n\t"	// Move: d4 = 0.0
		// load variable into registers
		"vld1.32	d0, [%1]!				\n\t"	// Load: s1 = m00
		"vld1.32	d1, [%1]! 				\n\t"	// Load: s2 = m01; s3 = m02
		"vld1.32  	d2, [%1]! 				\n\t"	// Load: s5 = m10
		"vld1.32	d3, [%1]! 				\n\t"	// Load: s6 = m11; s7 = m12
		"vld1.32	d4, [%1]!				\n\t"	// Load: s9 = m20
		"vld1.32	d5, [%1]! 				\n\t"	// Load: s10 = m21; s11 = m22


		"vneg.f32	s2, s2					\n\t"	// s2 = -(s2)
		"vmul.f32	s12, s6, s11			\n\t"	// s12 = s6 * s11
		"vmul.f32	s13, s7, s10			\n\t"	// s13 = s7 * s10
		"vmul.f32	s14, s5, s11			\n\t"	// s14 = s5 * s11
		"vmul.f32	s15, s7, s9				\n\t"	// s15 = s7, s9
		"vmul.f32	s16, s5, s10			\n\t"
		"vmul.f32	s17, s6, s9				\n\t"

		"vsub.f32	s12, s12, s13			\n\t"
		"vsub.f32	s14, s14, s15			\n\t"
		"vsub.f32	s16, s16, s17			\n\t"

		"vmul.f32	s1, s1, s12				\n\t"
		"vmul.f32	s2, s2, s14				\n\t"
		"vmul.f32	s3, s3, s16				\n\t"

		"vadd.f32	s1, s1, s2				\n\t"
		"vadd.f32	%0, s1, s3				\n\t"

		: "=w"(det)
		: "r"(matrix)
		: "q0", "q1", "q2", "q3", "q4", "q5");

	return det;
}

/** @fn 3x3 matrix determinant
*	3x3 matrix determinant w/ general C code
*/
float 
matrix_determinant_3x3_C(const float * m) {
	float c1 = m[1]*(m[6]*m[11] - m[7]*m[10]);
	float c2 = (-m[2])*(m[5]*m[11] - m[7]*m[9]);
	float c3 = m[3]*(m[5]*m[10] - m[6]*m[9]);

	return c1 + c2 + c3;
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

	volatile float a[2], b[2], r[2], r1[2];
	int i;
	int flag = 0;

	a[0] = 1;
	a[1] = 2;
	b[0] = 3;
	b[1] = 4;

	volatile float m[12] = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
	volatile float det[1];

	volatile float f[4] = {1.0, 2.0, 3.0, 4.0};
	float floatTstOut[4];

	volatile char char_input[32] = {1,1,1,1,2,2,2,2,
									3,3,1,1,6,6,0,0,
									0,1,0,3,0,5,0,7,
									0,1,0,3,0,5,0,7};
	volatile char char_output[16];

	timespec time1, time2;

	volatile float x[1] = {1.0};
	float sin_x;

#ifdef NEON
	// /// implementation of 3x3 det using neon
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	// for (i=0; i<1e8; i++) {
	// 	det = matrix_determinant_3x3_neon(m);
	// }
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	// std::cout << "3x3 det neon: " << (double)(diff(time1,time2).tv_nsec)/1000000 << " ms\n";
	
	// /// implementation of 3x3 det using regular C
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	// for (i=0; i<1e8; i++) {
	// 	det = matrix_determinant_3x3_C(m);
	// }
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	// std::cout << "3x3 det C: " << (double)(diff(time1,time2).tv_nsec)/1000000 << " ms\n";

	// /// implementation of float test function
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	// for (i=0; i<1e8; i++) {
	// 	expFn_float(f, floatTstOut);
	// }
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	// std::cout << "float test function: " << (double)(diff(time1,time2).tv_nsec)/1000000 << " ms\n";

	// /// implementation of char test function
	// /// implementation of float test function
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	// for (i=0; i<1e8; i++) {
	// 	expFn_char(char_input, char_output);
	// }
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	// std::cout << "char test function: " << (double)(diff(time1,time2).tv_nsec)/1000000 << " ms\n";

	/// implementation of sin function
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	// for (i=0; i<10; i++) {
	// 	asm_sine(x, sin_x);
	// }
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	// std::cout << "asm_sine function: " << (double)(diff(time1,time2).tv_nsec)/1000000 << " ms\n";
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	for (i=0; i < 1e8; i++) {
		foo(char_input, char_output);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	std::cout << "foo function takes: " << (double)(diff(time1, time2).tv_nsec)/1000000 << " ms\n";

#else

	
#endif
	std::cout << "\n";
    ///	execution result: 3x3 determinant neon version
    std::cout << "The determinant is by neon: " << det << std::endl;  
    ///	execution result: 3x3 determinant C version
    std::cout << "The determinant is by C: " << det << std::endl;
    /// execution result: float test function
    std::cout << "/*************************** test function ************************/" << std::endl;
    std::cout << "The float test function output is: " << floatTstOut[0] << ", " << floatTstOut[1] << ", "
    												   << floatTstOut[2] << ", " << floatTstOut[3] << std::endl;
	std::cout << "The char test function output is: " << static_cast<int>(char_output[0] )<< ", " << static_cast<int>(char_output[1] )<< ", "
    												   << static_cast<int>(char_output[2] )<< ", " << static_cast<int>(char_output[3] )<< ", "
    												   << static_cast<int>(char_output[4] )<< ", " << static_cast<int>(char_output[5] )<< ", "
    												   << static_cast<int>(char_output[6] )<< ", " << static_cast<int>(char_output[7] )<< ", "
    												   << static_cast<int>(char_output[8] )<< ", " << static_cast<int>(char_output[9] )<< ", "
    												   << static_cast<int>(char_output[10]) << ", " << static_cast<int>(char_output[11]) << ", "
    												   << static_cast<int>(char_output[12]) << ", " << static_cast<int>(char_output[13]) << ", "
    												   << static_cast<int>(char_output[14]) << ", " << static_cast<int>(char_output[15]) << std::endl;
    /// execution result: asm sine function
    std::cout << "asm_sine function generates output: " << sin_x << std::endl; 
    /// execution time result: asm_tstStall function
    std::cout << "asm_tstStall function generates output: " << det[0] << std::endl;

	return 0;
}				/* ----------  end of function main  ---------- */
