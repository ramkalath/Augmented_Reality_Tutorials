
CC = g++

CFLAGS = -g

INCLUDE = `pkg-config --cflags opencv`

LIBS = `pkg-config --libs opencv`

# enter filename without extension
FILENAME = camera_calibration

all: $(FILENAME).cpp
	@$(CC) $(CFLAGS) $(INCLUDE) $(FILENAME).cpp -o $(FILENAME) $(LIBS)

run:
	@./$(FILENAME)	

clean:
	rm ./$(FILENAME) 

