CC = g++

CFLAGS = -std=c++0x -g -Wall

INCLUDE = -I/usr/local/include/GLFW

LIBS = -lglfw3 -lm -lGLEW -lGL -lGLU -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread

# enter filename without extension
FILENAME = draw_rectangle

all: $(FILENAME).cpp
	@$(CC) $(CFLAGS) $(INCLUDE) $(FILENAME).cpp -o $(FILENAME) $(LIBS)

run:
	@./$(FILENAME)	

clean:
	rm ./$(FILENAME) 

