#include <iostream>
#include <time.h>

#include "include/helloworld.h"


//Time difference function
timespec diff(timespec start, timespec end)
{
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

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  This is the main function of the program 
 * =====================================================================================
 */

int main ( int argc, char *argv[] )
{
	
    timespec time1, time2;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    
        std::cout << "hello world!" << std::endl;
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    std::cout << "Hello world message:" << (double)(diff(time1,time2).tv_nsec)/1000000 << " ms\n";

	return 0;
}				/* ----------  end of function main  ---------- */
