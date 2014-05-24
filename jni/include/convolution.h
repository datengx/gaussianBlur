#ifndef	__CONVOLUTION_H
#define __CONVOLUTION_H
#include <iostream>

/** @Fn Convolution
*	convolution using 3x3 weighted patch
*/
float
convolution_3x3_neon() {
	float c;




	return c;
} // end of convolution_3x3_neon


/** @Fn Experiment function
*	test instruction based on float
*/
void
expFn_float(const float* a, float* r) {

	asm volatile(
		"vmov.f32	d3, #1.0		\n\t"
		"vld1.32	d0, [%0]!		\n\t"
		"vld1.32	d1, [%0]!		\n\t"
		"vadd.f32	d0, d0, d3		\n\t"

		
		"vst1.64	{d0,d1}, [%1]		\n\t"


		:: "r"(a), "r"(r)
		: "d1", "d2", "d3");
} // end of expFn_float

/** @Fn Experiment function
*	test function based on integers
*/
void 
expFn_char(const char* a, char* r) {
	int c;
	asm volatile(
		"vld2.8		{d0, d1}, [%0]!	\n\t"


		"vst1.8		{d0}, [%1]!	\n\t"
		"vst1.8		{d1}, [%1]!	\n\t"

		:: "r"(a), "r"(r)
		: "d1", "d2", "d3"

	);

} // end of expFn_int

#endif