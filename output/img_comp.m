%% Compare two images pixel-wise
a = imread('out.pgm');
b = imread('out_cv.pgm');
c = b-a;
d = a-b;
sum(c(:))/307200
sum(d(:))/307200
max(c(:))
mean(abs(c(:)))
hist(abs(c(:)),[0:1:4]);