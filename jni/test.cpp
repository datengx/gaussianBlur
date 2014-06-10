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
}


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
	// Make a border for OpenCV image
	top = left = bottom = right = 3;
	copyMakeBorder( dst_cv, dst_cv, top, bottom, left, right, cv::BORDER_CONSTANT, cv::Scalar(0,0,0) );

	/* -------------------------------------- Write Output -------------------------------------------------------------- */
	cv::imwrite("/data/data/test/out.pgm", dst);
	cv::imwrite("/data/data/test/out_cv.pgm", dst_cv);
	
	return 0;
} // End of Fn: Main