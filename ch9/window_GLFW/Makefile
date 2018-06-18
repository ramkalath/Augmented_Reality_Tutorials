CC = g++ -std=c++11

INCLUDE = -I/usr/local/include/GLFW

LIBS=-Wall -lglfw3 -lm -lGLEW -lGL -lGLU -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread

# enter filename without extension
FILENAME = window_GLFW

all: $(FILENAME).cpp
	$(CC) -g $(INCLUDE) $(FILENAME).cpp -o $(FILENAME) $(LIBS)

run:
	./$(FILENAME)

clean:
	rm ./$(FILENAME)
