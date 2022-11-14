CC := g++
CFLAGS := -std=c++17 -Wall -lgdi32 -lkernel32  -luser32 -llibopencv_core455 -llibopencv_highgui455 -llibopencv_imgproc455 -llibopencv_imgcodecs455 -municode
EXTRAFLAGS := -g -DDEBUG


buildPath := build
srcPath := src
objPath := objs

vpath %.cpp $(src)
vpath %.h  $(src)

.PHONY: clean build rebuild run


$(objPath)/%.o: %.cpp %.h
	$(CC) -o $(objPath)/$@ $< -c $(CFLAGS) $(EXTRAFLAGS)

$(buildPath)/run.exe: $(OBJ) $(srcPath)/capture.cpp
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRAFLAGS)


run: $(buildPath)/run.exe
	$<

build: $(buildPath)/run.exe

clean:
	cd $(buildPath) && DEL /F/Q/S *.* > NUL
	cd $(objPath) && DEL /F/Q/S *.* > NUL

rebuild:
	$(MAKE) clean 
	$(MAKE) build