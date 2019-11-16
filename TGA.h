#ifndef TGA_H_
#define TGA_H_


#include <fstream>
#include <iostream>
#include <string>


//TGA header specification http://www.paulbourke.net/dataformats/tga/
struct Header {

	char  idlength;
	char  colourmaptype;
	char  datatypecode;
	short colourmaporigin;
	short colourmaplength;
	char  colourmapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
    	char  bitsperpixel;
    	char  descriptor;
	bool hasalpha;

};

enum {R, G, B, A};

struct Pixel{
	unsigned char channel[4];
};

  
class TGA {
	public:
		bool load(const std::string& fileName);
		bool save(const std::string& fileName, Header *, Pixel *); 
		void loadpixel(Pixel *pixel, int byte, unsigned char * p);

		bool halfsize(const std::string& fileName);

		void readheader(std::ifstream&);
		bool readpixeldata(std::ifstream& tgaFile);
		~TGA();


	private:
		
		Header header;
		Pixel *pixeldata;  
};


#endif
