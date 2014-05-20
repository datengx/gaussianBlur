CFLAGS=-march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp 

all : complex_mult_neon complex_mult dotproduct dotproduct_neon

run :
	echo Benchmark complex multiplication
	time ./complex_mult
	time ./complex_mult_neon
	echo Benchmark dot product
	time ./dotproduct
	time ./dotproduct_neon

clean :
	rm -f complex_mult_neon complex_mult dot_product dot_product_neon

complex_mult_neon : complex_mult.c
	gcc $(CFLAGS) -DNEON -o $@ $<

complex_mult : complex_mult.c
	gcc $(CFLAGS) -o $@ $<

dotproduct : dotproduct.c
	gcc $(CFLAGS) -DU0 -o $@ $<

dotproduct_neon : dotproduct.c
	gcc $(CFLAGS) -DU1 -o $@ $<

