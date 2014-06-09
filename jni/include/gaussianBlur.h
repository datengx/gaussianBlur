#ifndef __GAUSSIANBLUR_H
#define __GAUSSIANBLUR_H

/** @namespace EyeMARS
 *   The EyeMARS namespace
 */
namespace EyeMARS {

void GaussianBlurNeon(unsigned char* src, unsigned char* dst, int height, int width);
void GaussianBlurC(unsigned char* src, unsigned char* dst, int height, int width);
void GaussianBlurStrip(unsigned char* src, unsigned char* dst, 
                        int w, int step, int step2, int numofiterations);


#endif
} // End of namespace: EyeMARS