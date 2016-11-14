SRC=../src
OBJ=../obj

INCLUDE=-I/usr/local/include \
		-I$(SRC)/../include

LIBSDIR=-L/usr/local/

sourcefile=$(wildcard $(SRC)/*.cpp)
objectfile=$(addprefix $(OBJ)/,$(subst .cpp,.o,$(notdir $(sourcefile))))
CXXFLAGS += -g -Wall -std=c++11
OPENCV_LIB = -l:libopencv_core.so.2.4 -l:libopencv_imgproc.so.2.4 -l:libopencv_highgui.so.2.4

test:$(objectfile)
	$(CXX) -o test $^ $(CXXFLAGS) $(LIBSDIR) $(OPENCV_LIB)

$(OBJ)/%.o:$(SRC)/%.cpp $(SRC)/%.cpp.d
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE) 

$(SRC)/%.cpp.d:$(SRC)/%.cpp
	$(CXX) $< -MM $(CXXFLAGS) $(INCLUDE) > $(OBJ)/$(notdir $<.d)

.PHONY:cc

cc:
	rm -rf ../obj/* test
