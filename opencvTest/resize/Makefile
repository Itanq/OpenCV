
INCLUDE=-I/usr/local/include
LIBEDIR=-L/usr/local/ \
		-l:libopencv_core.so.2.4 \
		-l:libopencv_imgproc.so.2.4 \
		-l:libopencv_highgui.so.2.4 \

CXXFLAG+= -g -Wall -std=c++11

test:main.o
	$(CXX) -o $@ $^ $(CXXFLAG) $(LIBEDIR)

main.o:main.cpp
	$(CXX) -o $@ -c $^ $(INCLUDE) 

clean:
	rm *.o test
