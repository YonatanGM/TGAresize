halfsize: halfsize.o TGA.o	
	g++ -Wall -o halfsize halfsize.o TGA.o

halfsize.o: halfsize.cpp TGA.h

TGA.o: TGA.cpp TGA.h


clean: 
	rm *.o halfsize
