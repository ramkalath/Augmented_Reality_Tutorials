
CC = g++

INCLUDE = `pkg-config --cflags opencv`

LIBS = `pkg-config --libs opencv`

# enter filename without extension
FILENAME = test_opencv

all: $(FILENAME).cpp
	@$(CC) -g $(INCLUDE) $(FILENAME).cpp -o $(FILENAME) $(LIBS)

run:
	@./$(FILENAME)

clean:
	rm ./$(FILENAME) 

