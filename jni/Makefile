CFLAGS=-march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp -static
LIB_CV=-L/usr/local/lib
CVLIBS=-lopencv_core -lopencv_imgproc -lopencv_highgui

testing_neon : main.o gaussianBlur.o
	g++ $(CFLAGS) main.o gaussianBlur.o -o testing_neon $(LIB_CV) $(CVLIBS)

run :
	echo Benchmark testing_neon
	time ./testing_neon

clean :
	rm -f main

main.o : main.cpp
	g++ $(CFLAGS) -DNEON -c main.cpp

gaussianBlur.o : ./src/gaussianBlur.cpp
	g++ $(CFLAGS) -DNEON -c ./src/gaussianBlur.cpp



