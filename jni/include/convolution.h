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
	unsigned char map[24] = {1,2,3,4,5,6,7,8,
							1,3,5,7,9,11,13,15,	
							0,2,4,6,8,10,12,14,
							}; // VTBL control vector

	asm volatile(
		/* ------------------- Load: input --------------------------------------- */
		"VLD1.8		{d0, d1}, [%0]!	\n\t"		// load data into vector {d0,d1} using bytes as unit
		"VLD1.8		{d26}, [%2]!	\n\t"	// 
		"VLD1.8		{d27}, [%2]!	\n\t"
		"VLD1.8		{d28}, [%2]!	\n\t"
		"VMOV.I32	q10, #0			\n\t"

		/* ------------------- Manipulate data ----------------------------------- */
		// "VSHLL.U8	q10, d1, #1		\n\t"	// Double: q10{d20,d21} = 2 * d1
		// "VADDW.U8	q10, q10, d0 	\n\t"	// Add: q10{d20,d21} = q10 + d0

		/* ------------------- VTBL test ------------------------------------------ */
		"VTBL.U8	d3, {d0, d1}, d26 \n\t"	// Map: d3 = {d0,d1} (1,2,3,4,5,6,7,8)
		"VTBL.U8	d4, {d0, d1}, d27 \n\t" // Map: d4 = {d0,d1} (1,3,5,7,9,11,13,15)
		"VTBL.U8	d5, {d0, d1}, d28 \n\t" // Map: d5 = {d0,d1} (0,2,4,6,8,10,12,14)
		"VSHLL.U8	q10, d3, #1 	  \n\t"
		/* ------------------------Store ------------------------------------------ */
		"VST1.8		{d20}, [%1]!	\n\t"	// 
		"VST1.8		{d21}, [%1]!		\n\t"
		""



		:: "r"(a), "r"(r), "r"(map)
		: "d0","d1", "d2", "d3", "d4", "d5", "q10", "d11", "d26", "d27", "d28", "memory"

	);

} // end of expFn_int

#endif