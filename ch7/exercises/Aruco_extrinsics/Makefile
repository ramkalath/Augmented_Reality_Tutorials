CC = g++

INCLUDE = `pkg-config --cflags opencv` -I/usr/local/include

LIBS = `pkg-config --libs opencv` -laruco

# enter filename without extension
FILENAME1 = main
FILENAME2 = marker_detect

all: $(FILENAME2).cpp $(FILENAME1).cpp
	@$(CC) -g $(INCLUDE) $(FILENAME2).cpp $(FILENAME1).cpp -o $(FILENAME1) $(LIBS)

run:
	@./run.sh

clean:
	rm ./$(FILENAME1) 

