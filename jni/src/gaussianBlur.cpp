#include <iostream>
#include "../include/gaussianBlur.h"

/** @namespace EyeMARS
 *   The EyeMARS namespace
 */
namespace EyeMARS {

#ifndef __ARM_NEON__
/// C based implementation of Gaussian Blur
void GaussianBlurC(unsigned char* src, unsigned char* dst, int height, int width) {
    const int kernel_single_tab[7] = {1, 6, 15, 20, 15, 6, 1}; 	// scale by 4096, row add up to 64
    int* horizontal = new int[width];

    unsigned char* row0 = src;
    unsigned char* row1 = src + width;
    unsigned char* row2 = src + 2 * width;
    unsigned char* row3 = src + 3 * width;
    unsigned char* row4 = src + 4 * width;
    unsigned char* row5 = src + 5 * width;
    unsigned char* row6 = src + 6 * width;

    // image too small to obtain 
    if (height < 7 || width < 7) {
    	dst = src;
        return ;
    }
    // normal size image: >= 7
    for (int i = 3; i < height - 3; i++) {
    	for (int j = 0; j < width; j++) {
    		horizontal[j] = row0[j] + row6[j] + 6*(row1[j] + row5[j]) + 15*(row2[j] + row4[j]) + 20*row3[j];
    	}
    	for (int k = 0; k < width - 6; k++) {
    		dst[i * width + k + 3] = (horizontal[k] + 6*horizontal[k + 1] + 15*horizontal[k + 2] + 20*horizontal[k + 3]
                                   + 15*horizontal[k + 4] + 6*horizontal[k + 5] + horizontal[k + 6]) >> 12;
    	}
    	row0 = row1;
    	row1 = row2;
    	row2 = row3;
    	row3 = row4;
    	row4 = row5;
    	row5 = row6;
    	row6 += width;
    }
    delete [] horizontal;
} // End of GaussianBlur
#endif

#ifdef __ARM_NEON__
/// ARM NEON based implementation of gaussian blur using 7x7 kernel
void GaussianBlurNeon(unsigned char* src, unsigned char* dst, int h, int w) {
    // Parameters for GaussianBlurStrip
    int step = w;
    int step2 = 0;
    dst += (3 * w) + 3;
    int strips_num = (w - 6) / 2;
    int numofiterations = (h - 6);
    // Loop for computing Gaussian Blur
    for (int i = strips_num; i--;) {
        GaussianBlurStrip(src, dst, w, step, step2, numofiterations);
        src += 2;
        dst += 2;
    }
} // End of GaussianBlur

/// ARM NEON based implementation of constructing one strip of gaussian blurred pixels 
void GaussianBlurStrip(unsigned char* src, unsigned char* dst, 
                        int w, int step, int step2, int numofiterations) {
    volatile unsigned char map[32] = {1,0,6,0,15,0,20,0,
                                      15,0,6,0,1,0,0,0,
                                      0,0,1,0,6,0,15,0,
                                      20,0,15,0,6,0,1,0};
    /* ------------------------------------ Start of Assembly ---------------------------------------------- */
    asm volatile(
    /* ------------------------------------------- Load Parameters ------------------------------------------ */
    "mov        r0, %2; "       // Move: r0 = step down for loading next line of image
    "mov        r1, %0; "       // Move: r 1 = load pointer
    "mov        r2, %4; "       // Move: r2 = numofiterations to compute one strip of gaussian blur
    "mov        r3, %1; "       // Move: r3 = store pointer
    "mov        r4, %6; "       // Move: r4 = step after storing the second result
    "VLD1.8     {d14,d15}, [%5]!; "    // Load: q7 = (1,0, 6,0, 15,0, 20,0, 15,0, 6,0, 1,0, 0,0)
    "VLD1.8     {d16,d17}, [%5]!; "     // Load: q8 = (0,0, 1,0, 6,0, 15,0, 20,0, 15,0, 6,0, 1,0)
    "VMOV.I8    d7, #6; "
    "VMOV.I8    d8, #15; "
    "VMOV.I8    d9, #20; "
    /* ------------------------------------ Load Line 0 ~ 6 pixels (0-6) ------------------------------------ */    
    "VLD1.8     d0, [r1], r0; " // Load: d0 = pixels 0 ~ 7 of Line 0
    "VLD1.8     d1, [r1], r0; " // Load: d1 = pixels 0 ~ 7 of Line 1
    "VLD1.8     d2, [r1], r0; " // Load: d2 = pixels 0 ~ 7 of Line 2 
    "VLD1.8     d3, [r1], r0; " // Load: d3 = pixels 0 ~ 7 of Line 3
    "VLD1.8     d4, [r1], r0; " // Load: d4 = pixels 0 ~ 7 of Line 4
    "VLD1.8     d5, [r1], r0; " // Load: d5 = pixels 0 ~ 7 of Line 5
    /* ------------------------------Loop for each next 1 lines -------------------------------------------- */
    ".loop:;"
    "VLD1.8     d6, [r1], r0; " // Load: d6 = pixels 0 ~ 7 of Line 6
    "PLD        [r1]; "         // Preload: one line in cache
    "sub        r2, r2, #1; "   // decrement: numofinteration -= 1;
    /* ------------------------------ Vertical Convolution ----------------------------------- */
    "VADDL.U8   q11, d0, d6; "  // Add Long: q11{d22, d23} = d0 + d6 (q11 = Line0[0:7] + Line6[0:7])
    "VMLAL.U8   q11, d1, d7; "// Multiply Accumulate Long: q11 += d1 * d7[1] (q11 += d1 * 6, 6 is a scalar)
    "VMOV.U8    d0, d1; "       // Shift Up: Line 1 -> new Line 0 
    "VMLAL.U8   q11, d2, d8; " // Multiply Accumulate Long: q11 += d2 * d7[2] (q11 += d2 * 15, 15 is a scalar)
    "VMOV.U8    d1, d2; "       //Shift Up: Line 2 -> new Line 1
    "VMLAL.U8   q11, d3, d9; " // Multiply Accumulate Long: q11 += d3 * d7[3] (q11 += d3 * 20, 20 is a scalar)
    "VMOV.U8    d2, d3; "       // Shift Up: Line 3 -> new Line 2
    "VMLAL.U8   q11, d4, d8; " // Multiply Accumulate Long: q11 += d4 * d7[2] (q11 += d4 * 15, 15 is a scalar)
    "VMOV.U8    d3, d4; "       // Shift Up: Line 4 -> new Line 3
    "VMLAL.U8   q11, d5, d7; " // Multiply Accumulate Long: q11 += d5 * d7[1] (q11 += d5 * 6, 6 is a scalar)
    "VMOV.U8    d4, d5; "       // Shift Up: Line5 -> new Line 4
    "VMOV.U8    d5, d6; "       // Shift Up: Line6 -> new Line 5
    /* ------------------------------ Horizontal Convolution (1st Result) ----------------------------------- */
    "VMULL.U16  q12, d22, d14; " // Multiply Long: q12 = d22 * d14 (q12=(1*vertical[0], 6*vertical[1], 15*vertical[2], 20*vertical[3]))
    "VMULL.U16  q13, d23, d15; " // Multiply Long: q13 = d23 * d15 (q13=(15*vertical[4], 6*vertical[5], 1*vertical[6], 0))
    "VPADD.U32  d10, d24, d25; " // Pairwise Add: d10(s20=s48+s49, s21=s50+s51) (d10=(vertical[0]+6*vertical[1], 15*vertical[2]+20*vertical[3]))
    "VPADD.U32  d11, d26, d27; " // Pairwise Add: d11(s22=s52+s53, s23=s54+s55) (d11=(15*vertical[4]+6*vertical[5], vertical[6]))
    "VPADD.U32  d28, d10, d11; " // Pairwise Add: d28(s56=s20+s21, s57=s22+s23) (d28=(vertical[0]+6*vertical[1]+15*vertical[2]+20*vertical[3],
                                 //                                                     15*vertical[4]+6*vertical[5]+vertical[6]
    /* ------------------------------ Horizontal Convolution (2nd Result) ----------------------------------- */
    "VMULL.U16  q12, d22, d16; " // Multiply Long: q12 = d22 * d16 (q12=(1*vertical[1], 6*vertical[2], 15*vertical[3], 20*vertical[4]))
    "VMULL.U16  q13, d23, d17; " // Multiply Long: q13 = d23 * d17 (q13=(15*veitical[5], 6*vertical[6], veitical[7], 0))
    "VPADD.U32  d10, d24, d25; " // Pairwise Add: d10(s20=s48+s49, s21=s50+s51) (d10=(vertical[1]+6*vertical[2], 15*vertical[3]*20*vertical[4]))
    "VPADD.U32  d11, d26, d27; " // Pairwise Add: d11(s22=s52+s53, s23=s54+s55) (d11=(15*vertical[5]+6*vertical[6], vertical[7]))
    "VPADD.U32  d29, d10, d11; " // Pairwise Add: d29(s58=s20+s21, s59=s22+s23) (d29=(vertical[1]+6*vertical[2]+15*vertical[3]+20*vertical[4],
                                 //                                                     15*vertical[5]+6*vertical[6]+vertical[7]
    /* ------------------------------ Store 2 Results ----------------------------------- */
    "VPADD.U32  d30, d28, d29; " // Pairwise Add: d30=(v[0]+6*v[1]+15*v[2]+20*v[3]+15v[4]+6*v[5]+v[6],v[1]+6*v[2]+15*v[3]+20*v[4]+15v[5]+6v[6]+v[7])
    "VRSHR.U32  d30, d30, #12; "  // Shift Right Round: d30 = d30 >> 12 (d30 / 4096)
    "VST1.8     {d30[0]}, [r3]!; " // Store: 1st Result
    "VST1.8     {d30[4]}, [r3], r4; " // Store: 2nd Result
    "cmp        r2, #0; "       // Compare: (numofiteration == 0)?
    "bne        .loop; "        // Branch If Not Zero; to .loop

    :: "r"(src), "r"(dst), "r"(step), "r"(step2), "r"(numofiterations), "r"(map), "r"(w-1)
    : "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q10", "q11", "q12", "q13", "q14", "q15", "r0", "r1", "r2", "r3", "r4", "r5" 
    );
    /* ------------------------------------ End of Assembly ---------------------------------------------- */
} // End of GaussianBlurStrip
#endif

} // End of namespace: EyeMARS