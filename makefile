main: halfsize.o TGA.o
	g++ -Wall -o main halfsize.o TGA.o

halfsize: halfsize.cpp TGA.h
	g++ -Wall -o halfsize.cpp halfsize.h

TGA: TGA.cpp TGA.h
	g++ -Wall -o TGA.cpp TGA.h

clean: 
	rm *.o main  
