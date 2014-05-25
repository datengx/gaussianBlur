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
		"VMOV.f32	d3, #1.0		\n\t"
		"VLD1.32	d0, [%0]!		\n\t"
		"VLD1.32	d1, [%0]!		\n\t"
		"VADD.f32	d0, d0, d3		\n\t"

		
		"VST1.64	{d0,d1}, [%1]		\n\t"


		:: "r"(a), "r"(r)
		: "d1", "d2", "d3");
} // end of expFn_float

/** @Fn Experiment function
*	test function based on integers
*/
void 
expFn_char(const char* a, char* r) {
	int c;
	unsigned char map[8] = {1,2,3,4,5,6,7,8};

	asm volatile(
		/* ------------------- Load: input ------------------- */
		"VLD1.8		{d0, d1}, [%0]!	\n\t"
		"VLD1.8		{d2}, [%2]!		\n\t"
		/* ------------------- Manipulate data --------------- */
		// "VSHLL.U8	q10, d1, #1		\n\t"	// Double: q10{d20,d21} = 2 * d1
		// "VADDW.U8	q10, q10, d0 	\n\t"	// Add: q10{d20,d21} = q10 + d0

		/* ------------------- VTBL test --------------- */
		"VTBL.U8		d3, {d0}, d2 \n\t"	// MAP: d20 = {d0,d1} (1,2,3,4,5,6,7,8)

		"VST1.8		{d2}, [%1]!	\n\t"
		"VST1.8		{d3}, [%1]!	\n\t"


		:: "r"(a), "r"(r), "r"(map)
		: "d0","d1", "d2", "d3", "q10", "d11", "memory"

	);

} // end of expFn_int

#endif