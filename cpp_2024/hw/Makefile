# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive -Wno-deprecated -std=c++14
LDFLAGS=
DEBUG=0
DEPS=Matrix.h colors.h

all:: memory.exe testMatrix.exe

memory.exe: memory.o Matrix.o ttymodes.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

testMatrix.exe: testMatrix.o Matrix.o 
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe 
	rm -f *.o *~ *.stackdump
