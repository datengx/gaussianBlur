#include <iostream>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "include/helloworld.h"
#include "include/convolution.h"
#include "include/gaussianBlur.h"

/* @Fn diff
*	Time difference function
*/
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
} // End of Fn: diff

/* @Fn padBorder
*	pad border in image with chosen gray scale value
*/
void
padBorder(cv::Mat src, int top, int bottom, int left, int right, unsigned char v) {
	int h = src.rows;
	int w = src.cols;
	// printf("Rows: %d.\n", h);
	// printf("Cols: %d.\n", w);

	// Check if image is too small to make border
	if (w < (left + right) || h < (top + bottom)) {
		perror("Img too small to put the border.\n");
		return ;
	}
	// Fill the top border
	for (int i = 0; i < 640; i++) {
		for (int j = 0; j < top; j++) {
			src.data[j*w + i] = v;
		}
	}
	// Fill the bottom border
	for (int i = 0; i < 640; i++) {
		for (int j = 1; j <= bottom; j++) {
			src.data[(h - j)*w + i] = v;
		}
	}
	// Fill the left border
	for (int i = 0; i < 480; i++) {
		for (int j = 0; j < left; j++) {
			src.data[i*w + j] = v;
		}
	}
	// Fill the right border
	for (int i = 0; i < 480; i++) {
		for (int j = 1; j <= right; j++) {
			src.data[i*w + (w - j)] = v;
		}
	}

} // End of Fn: padBorder

/* @Fn main 
*  	test function for Gaussian Blur
*/
int 
main ( int argc, char *argv[] ) {
	cv::Mat src;
	cv::Mat dst;
	cv::Mat dst_cv;
	timespec time1, time2;
	int top,left,bottom,right;

	dst.create(480,640, CV_8U);
	src = cv::imread("/data/data/test/00000000.pgm", CV_LOAD_IMAGE_GRAYSCALE);
	// cv::GaussianBlur(src, array, cv::Size(7,7), 2, 2);

	/* ------------------------------------- Testing GaussianBlur Neon ----------------------------------------------- */
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	for (int i=0; i < 10; i++) {
		EyeMARS::GaussianBlurNeon(src.data,dst.data,480,640);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	std::cout << "GaussianBlurNeon takes: " << (double)(diff(time1, time2).tv_nsec)/1000000 << " ms\n";
	/* ------------------------------------- Testing GaussianBlur OpenCV ----------------------------------------------- */
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
	for (int i=0; i < 10; i++) {
		cv::GaussianBlur(src,dst_cv,cv::Size(7,7),1.21,1.21);
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
	std::cout << "GaussianBlur (OpenCV version) takes: " << (double)(diff(time1, time2).tv_nsec)/1000000 << " ms\n";

	padBorder(dst_cv, 3, 3, 3, 3, 0);

	/* ------------------------------------- Border Checking for Output Image ----------------------------------------------- */ 
	/*
	for (int i = 0; i < 640; i++) 
		std::cout << (int)dst_cv.data[i] << ' ';
	std::cout << std::endl;
	for (int j = 0; j < 640; j++)
		std::cout << (int)dst.data[j+640*3] << ' ';
	std::cout << std::endl;
	*/
	/* -------------------------------------- Write Output -------------------------------------------------------------- */
	cv::imwrite("/data/data/test/out.pgm", dst);
	cv::imwrite("/data/data/test/out_cv.pgm", dst_cv);
	
	return 0;
} // End of Fn: Main