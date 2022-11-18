CC := g++
CFLAGS := -std=c++17 -Wall -g -DDEBUG
LIBS :=  -ld3d11 -lgdi32 -lkernel32 -luser32 -lws2_32 -llibopencv_core455 -llibopencv_highgui455 -llibopencv_imgproc455 -llibopencv_imgcodecs455
EXENAME := run


buildPath := build
srcPath := src
socketPath := src\Socket
objPath := OBJ

OBJ = socket.o tcpSocket.o windowCaptrue.o server.o client.o
OBJ := $(addprefix $(objPath)/, $(OBJ))

vpath %.cpp $(srcPath):$(socketPath)
vpath %.h   $(srcPath):$(socketPath)

.PHONY: clean build rebuild run server client

$(objPath)/%.o: %.cpp %.h
	$(CC) -o $@ $< -c $(CFLAGS)

$(buildPath)/$(EXENAME).exe: $(OBJ) $(srcPath)/main.cpp
	$(CC) -o $@ $^ $(CFLAGS) $(CLIENTFLAGS) $(SERVERFLAGS) $(LIBS)

run: $(buildPath)/$(EXENAME).exe

server: 
	$(MAKE) run SERVERFLAGS="-DSERVER" EXENAME="server"

client: 
	$(MAKE) run CLIENTFLAGS="-DCLIENT" EXENAME="client"


build:
	$(MAKE) run EXENAME="run"

clean:
	cd $(buildPath) && DEL /F/Q/S *.* > NUL
	cd $(objPath) && DEL /F/Q/S *.* > NUL

rebuild:
	$(MAKE) clean 
	$(MAKE) build