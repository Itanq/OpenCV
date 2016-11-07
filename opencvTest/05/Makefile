
OBJ = $(cppfile)
INCLUDE = -I/usr/local/include
LIBES = -L/usr/local/lib

CXXFLAGS += -g -Wall -std=c++11
opencv_lib = -l:libopencv_core.so.2.4 -l:libopencv_highgui.so.2.4 -l:libopencv_imgproc.so.2.4

test: $(OBJ).o
	$(CXX) -o test $^ $(CXXFLAGS) $(LIBES) $(opencv_lib)

$((OBJ).o: $(OBJ).cpp
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(INCLUDE)

.PHONY: clean

clean:
	rm -rf test *.o


