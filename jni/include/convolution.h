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

// void
// asm_sine(const float* a, float r) {
// 	float map[8] = {3.14159265, 1.27323954, 0.405284735, 0.0, -3.14159265, 
// 					6.28318531, 0.0, 0.0};

// 	asm volatile(
// 		/* -------------------- move coefficients ------------------------ */
// 		// "vld1.32 	s10, [%0]!		\n\t"	
// 		"vld1.32 	d0, [%2]!	\n\t"	// r0 = pi
// 		"vld1.32 	d1, [%2]!	\n\t"	// 1st order coefficient
// 		"vld1.32 	d2, [%2]!	\n\t"	// 2nd order coefficient
// 		"vld1.32	d3, [%2]!	\n\t"

// 		/* -------------------- take input within range ------------------ */
// 		".RANGE_DET_LOWER:		\n\t"
// 		"vcmp.f32	s10, s4 		\n\t"	// if a > -3.14159265
// 		"bgt	.RANGE_DET_UPPER	\n\t"	// branch to upper bound check
// 		"vadd.f32 	s10, s10, s5	\n\t"	// else a += 6.28318531
// 		"b 		.RANGE_DET_LOWER \n\t"		// branch back for testing

// 		".RANGE_DET_UPPER:		\n\t"
// 		"vcmp.f32 	s10, s0		\n\t"		// if a < 3.1415926
// 		"blt	.COMPUTE		\n\t"		// branch to computation
// 		"vsub.f32 	s10, s10, s5	\n\t"	// else a -= 6.28318531
// 		"b 		.RANGE_DET_UPPER 	\n\t"	// branch back for testing
// 		/* -------------------- compute subroutine ----------------------- */
// 		".COMPUTE:				\n\t"
// 		"vcmp.f32 	s10, s6 	\n\t"
// 		"vldrgt.f32	s10, %1	\n\t"
// 		"vldrlt.f32	s10, %1	\n\t"


// 		:: "r"(a), "r"(r), "r"(map)
// 		: "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s10", "memory"
// 		);
// } // end of asm_sine

void 
asm_cosine(const float* a, float r) {
	float map[8] = {3.14159265, 1.27323954, 0.405284735, 0.0, -3.14159265, 
					6.28318531, 0.0, 0.0};

	asm volatile(
		""

		:: "r"(a), "r"(r), "r"(map)
		:	"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s10", "memory"
		);
} // end of asm_cosine

void 
asm_tstStall(const float* a, float* r) {
	float map[5] = {1.0,2.0,3.0,4.0,5.0};

	asm volatile(
		"VLD1.32	{d0}, [%0]!; "
		"VLD1.32	{d1}, [%0]!; "
		/*  ------------------------- test different stall profile ------------------------ */ 
		"VADDL.U8	q1, d0, d1; " // ADD: q1 = d0 + d1
		"VST1.8		{d2}, [%1]!; "
		"VST1.8		{d3}, [%1]!; "


		:: "r"(a), "r"(r), "r"(map)
		:	"q0", "q1", "q2", "q3"
		);
} // end of asm_tstStall

void 
foo(const unsigned char* a, unsigned char* r) {
	volatile unsigned char map[16] = {1,0,6,0,15,0,20,0,
									  15,0,6,0,1,0,0,0};
	asm volatile(
		"mov 		r0, #16; "	// load step
		"mov 		r1, #8; "	// Load step
		"mov 		r2, #1; "	// Store step
		"VLD1.8		{d0}, [%0], r0; "
		"VLD1.8		{d1}, [%0], r1; "
		"VLD1.8		{d3}, [%2]!; "
		// "VMOV.U8		q1, #0; "
		/*  ------------------------- test different stall profile ------------------------ */ 
		// "VADDL.U8	q1, d1, d1; " 		// ADD: q1 = d0 + d1
		/*
		"VADDL.U8	q1, d0, d0; "	// q1 = d0 + d0
		"VADD.U16	q2, q0, q0; "	// q1 = 2 * q1
		"VADD.U16	q3, q0, q0; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q3, q3, q3; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		*/
		/*
		"VADDL.U8	q1, d0, d0; "	// q1 = d0 + d0
		"VADD.U16	q2, q0, q0; "	// q1 = 2 * q1
		"VADD.U16	q3, q0, q0; "	// q1 = 2 * q1
		"VADD.U16	q4, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q3, q3, q3; "	// q1 = 2 * q1
		"VADD.U16	q4, q4, q4; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q3, q3, q3; "	// q1 = 2 * q1
		"VADD.U16	q4, q4, q4; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q3, q3, q3; "	// q1 = 2 * q1
		"VADD.U16	q4, q4, q4; "	// q1 = 2 * q1
		"VADD.U16	q1, q1, q1; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q3, q3, q3; "	// q1 = 2 * q1
		"VADD.U16	q4, q4, q4; "	// q1 = 2 * q1
		*/
		
		/*
		"VADDL.U8	q1, d0, d0; "	// q1 = d0 + d0
		"VADD.U16	q2, q0, q0; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q2, q2, q2; "	// q1 = 2 * q1
		"VADD.U16	q1, q2, q2; "	// q1 = 2 * q1
		*/
		/*
		// testing VPADD 
		"VMULL.U16	q1, d0, d4; "
		"VMULL.U16	q3, d0, d4; "
		"VPADD.U32	d8, d2, d3; "
		"VPADD.U32	d9, d6, d7; " 
		"VPADD.U32	d1, d8, d9; "
		*/
		/*
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d1, d1; "	// Add: d3 = d2 * 2
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d1, d1; "	// Add: d3 = d2 * 2
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d1, d1; "	// Add: d3 = d2 * 2
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d1, d1; "	// Add: d3 = d2 * 2
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d1, d1; "	// Add: d3 = d2 * 2
		*/
		/*
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d2, d2; "	// Add: d3 = d2 * 2
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d2, d2; "	// Add: d3 = d2 * 2
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d2, d2; "	// Add: d3 = d2 * 2
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d2, d2; "	// Add: d3 = d2 * 2
		"VMLA.U8	d2, d0, d0; "	// Add: d2 = d0 * 2
		"VMLA.U8	d1, d2, d2; "	// Add: d3 = d2 * 2
		*/
		"VRSHR.U32	d3, d0, #16; "	// Shift Right Round: d0 >> 8
		"VRSHR.U32	d4, d1, #16; "   // Shift Right Round: d1 >> 8
		//"VREV64.32	d4, d3; "		// Swap: s6 <-> s7
		//"VZIP.8		d3, d4; "	// Zip: 
		//"VREV32.8	d3, d3; "		// Rotate the result
		//"VREV32.8	d4, d4; "		// Rotate the result
		"VST1.8		{d3[0]}, [%1], r2; "
		"VST1.8		{d3[5]}, [%1], r2; "
		//"VST1.8		{d4}, [%1], r2"
		:: "r"(a), "r"(r), "r"(map)
		:	"q0", "q1", "q2", "q3", "q4"
		);

}

#endif	/// check NEON definition
#endif
