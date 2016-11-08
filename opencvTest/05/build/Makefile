
SRC=$(ProDir)/src
OBJ=$(ProDir)/obj

INCLUDE=-I/usr/local/include \
		-I$(ProDir)/include

LIBSDIR=-L/usr/local/lib

sourcefile=$(wildcard $(SRC)/*.cpp)
objfile=$(addprefix $(OBJ)/,$(subst .cpp,.o,$(notdir $(sourcefile))))

opencv_lib=-l:libopencv_core.so.2.4 -l:libopencv_highgui.so.2.4 -l:libopencv_imgproc.so.2.4

CXXFLAGS += -g -Wall -std=c++11

test:$(objfile)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBSDIR) $(opencv_lib)

$(OBJ)/%.o:$(SRC)/%.cpp $(SRC)/%.cpp.d
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(INCLUDE)

$(SRC)/%.cpp.d:$(SRC)/%.cpp
	$(CXX) $< -MM $(CXXFLAGS) $(INCLUDE) > $(OBJ)/$(notdir $<.d)

.PHONY: clean

clean:
	rm -rf *.o test
