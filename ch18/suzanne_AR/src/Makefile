CC = g++

CFLAGS = -std=c++0x -g 

INCLUDE = `pkg-config --cflags opencv` -I/usr/local/include -I/usr/local/include/GLFW -I/usr/include/SOIL

LIBS = -lglfw3 -lm -lGLEW -lGL -lGLU -lSOIL -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread `pkg-config --libs opencv` -laruco -lassimp

# enter filename without extension
FILENAME1 = main
FILENAME2 = ../include/marker_detect
FILENAME3 = ../include/shader

all: $(FILENAME3).cpp $(FILENAME2).cpp $(FILENAME1).cpp
	$(CC) $(CFLAGS) $(INCLUDE) $(FILENAME3).cpp $(FILENAME2).cpp $(FILENAME1).cpp -o ../bin/$(FILENAME1) $(LIBS)

run:
	@./run.sh

clean:
	rm ../bin/./$(FILENAME1) 

