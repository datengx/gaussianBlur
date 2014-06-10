#include <iostream>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "include/helloworld.h"
#include "include/convolution.h"
#include "include/gaussianBlur.h"

/* @Fn main 
*  	test function for Gaussian Blur
*/

int 
main ( int argc, char *argv[] ) {
	cv::Mat src;
	cv::Mat dst;

	dst.create(480,640, CV_8U);
	src = cv::imread("/data/data/test/00000000.pgm", CV_LOAD_IMAGE_GRAYSCALE);
	// cv::GaussianBlur(src, array, cv::Size(7,7), 2, 2);
	EyeMARS::GaussianBlurNeon(src.data,dst.data,480,640);
	printf("%p.\n", &(src.data[0]));
	printf("%p.\n", &(src.data[1]));

	cv::imwrite("/data/data/test/out.pgm", dst);
	
	return 0;
} // End of Fn: Main