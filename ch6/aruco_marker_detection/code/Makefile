CC = g++

INCLUDE = `pkg-config --cflags opencv` -I/usr/local/include

LIBS = `pkg-config --libs opencv` -laruco

# enter filename without extension
FILENAME = marker_detection

all: $(FILENAME).cpp
	@$(CC) -g $(INCLUDE) $(FILENAME).cpp -o $(FILENAME) $(LIBS)

run:
	@./run.sh

clean:
	rm ./$(FILENAME) 

