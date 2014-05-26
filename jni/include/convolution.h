#ifndef	__CONVOLUTION_H
#define __CONVOLUTION_H
#include <iostream>
#include <cmath>

#ifdef NEON
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
		"VADDW.U8	q10, q10, d3	  \n\t" // Add: q10 = q10{d20,d21} + d3
		/* -------------------- test immediate moving ----------------------------- */
		"VMOV.I32	d10, #0x11111111		  \n\t"	// Mov: 17,17,17,17 to d10
		/* -------------------- test preload register ----------------------------- */

		/* ------------------------Store ------------------------------------------ */
		"VST1.8		{d10}, [%1]!	\n\t"	// 
		"VST1.8		{d21}, [%1]!		\n\t"
		""
		



		:: "r"(a), "r"(r), "r"(map)
		: "d0","d1", "d2", "d3", "d4", "d5", "q10", "d10", "d11", "d26", "d27", "d28", "memory"

	);

} // end of expFn_int

void
asm_sine(const float* a, float r) {
	float map[8] = {3.14159265, 1.27323954, 0.405284735, 0.0, -3.14159265, 6.28318531, 0.0, 0.0};

	asm volatile(
		/* -------------------- move coefficients ------------------------ */
		"vmov.f32 	s10, %0		\n\t"	
		"vld1.f32 	d0, [%2]!	\n\t"	// r0 = pi
		"vld1.f32 	d1, [%2]!	\n\t"	// 1st order coefficient
		"vld1.f32 	d2, [%2]!	\n\t"	// 2nd order coefficient
		"vld1.f32	d3, [%2]!	\n\t"

		/* -------------------- take input within range ------------------ */
		".RANGE_DET_LOWER:		\n\t"
		"vcmp.f32	s10, s4 		\n\t"	// if a > -3.14159265
		"bgt	.RANGE_DET_UPPER	\n\t"	// branch to upper bound check
		"vadd.f32 	s10, s10, s5	\n\t"	// else a += 6.28318531
		"b 		.RANGE_DET_LOWER \n\t"	

		".RANGE_DET_UPPER:		\n\t"
		"vcmp.f32 	s10, s0		\n\t"	// if a > 3.1415926
		"blt	.COMPUTE	\n\t"	// branch to computation
		"vsub.f32 	s10, s10, s5	\n\t"	// else a -= 6.28318531
		"b 		.RANGE_DET_UPPER 	\n\t"
		/* -------------------- compute subroutine ----------------------- */
		".COMPUTE:				\n\t"
		"vcmp.f32 	s10, s6 		\n\t"
		"vldrgt.f32	s10, [%1]	\n\t"
		"vldrlt.f32	s10, [%1]	\n\t"

		:: "r"(a), "r"(r), "r"(map)
		: "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s10", "memory"
		);
} // end of asm_sine

void 
asm_cosine(const float* a, float r) {

	asm volatile(
		""
		);
} // end of asm_cosine

#endif	/// check NEON definition
#endif