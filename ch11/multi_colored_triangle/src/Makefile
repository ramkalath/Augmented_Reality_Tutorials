CC = g++

CFLAGS = -std=c++0x -g -Wall

INCLUDE = -I/usr/local/include/GLFW

LIBS = -lglfw3 -lm -lGLEW -lGL -lGLU -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread

# enter filename without extension
FILENAME1 = main
FILENAME2 = ../include/shader

all: $(FILENAME2).cpp $(FILENAME1).cpp
	@$(CC) $(CFLAGS) $(INCLUDE) $(FILENAME2).cpp $(FILENAME1).cpp -o ../bin/$(FILENAME1) $(LIBS)

run:
	@../bin/./$(FILENAME1)	

clean:
	rm ../bin/./$(FILENAME1) 

