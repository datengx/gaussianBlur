CFLAGS=-march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp 

all : testing_neon

run :
	echo Benchmark testing_neon
	time ./testing_neon

clean :
	rm -f main

testing_neon : main.cpp
	g++ $(CFLAGS) -DNEON -o $@ $<



