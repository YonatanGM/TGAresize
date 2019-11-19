#include "TGA.h"

TGA::~TGA() {
	delete [] pixeldata;
}


bool TGA::load(const std::string& fileName) {
 
	std::cout << "Reading " << "\"" << fileName.c_str() << "\"" << std::endl;
	std::ifstream tgaFile(fileName, std::ifstream::binary);

	if (!tgaFile.is_open()) {
		std::cerr << "Error opening" << fileName.c_str() << std::endl;
		return false;
	}
	
	readheader(tgaFile);

	if (header.bitsperpixel == 16 || header.bitsperpixel == 32) header.hasalpha = true;
	else header.hasalpha = false;

   	if (header.datatypecode != 2 ) {
     	std::cerr << "uncompressed RGB only" << std::endl;
   		return false;	
   	}
	if (!(header.bitsperpixel == 16 || header.bitsperpixel == 24 || header.bitsperpixel ==32)) {
		std::cerr << "invalid pixel depth" << std::endl;
      	return false;
	}
   	if (header.colourmaptype != 0) {   //no support for color mapped  
		std::cerr << "color mapped TGA not supported" << std::endl;
     	return false;
   	}


	if(!readpixeldata(tgaFile)) return false;

	std::cout << "Done." << std::endl;
	
	return true;
	
}

bool TGA::readpixeldata(std::ifstream& tgaFile) {
	tgaFile.seekg(18, std::ios::beg);   //header part is 18 bytes
	//skip color map part 
	tgaFile.seekg(header.idlength + header.colourmaptype * header.colourmaplength , std::ios::cur); 

	pixeldata = new Pixel[header.width*header.height];
	unsigned char p[4];    //RGBA values 
	int nbytes = header.bitsperpixel / 8;

	for(int i=0; i <  header.width * header.height; i++) { 
		if (!tgaFile.read((char *)&p[0], nbytes)) {    
			std::cout << "Read error" << std::endl;
			return false;
	     }
	     loadpixel(&pixeldata[i],nbytes, p);		
	
	}

	return true; 
}


void TGA::readheader(std::ifstream& tgaFile) {
	tgaFile.seekg(0, std::ios::beg);
	
	tgaFile.read (&header.idlength, sizeof(header.idlength));
	tgaFile.read (&header.colourmaptype, sizeof(header.colourmaptype));
	tgaFile.read (&header.datatypecode, sizeof(header.datatypecode));
	
	//color map specification 
	tgaFile.read ((char*) &header.colourmaporigin, sizeof(header.colourmaporigin));
	tgaFile.read ((char*) &header.colourmaplength, sizeof(header.colourmaplength));
	tgaFile.read (&header.colourmapdepth, sizeof(header.colourmapdepth));

	//image specification
	tgaFile.read ((char*) &header.x_origin, sizeof(header.x_origin));
	tgaFile.read ((char*) &header.y_origin, sizeof(header.y_origin));
	tgaFile.read ((char*) &header.width, sizeof(header.width));
	tgaFile.read ((char*) &header.height, sizeof(header.height));
	tgaFile.read (&header.bitsperpixel, sizeof(header.bitsperpixel));
	tgaFile.read (&header.descriptor, sizeof(header.descriptor));

}

void TGA::loadpixel(Pixel *pixel,int bytes, unsigned char *p) {
	if (bytes == 4) {
		pixel->channel[R] = p[2];
      		pixel->channel[G] = p[1];
      		pixel->channel[B] = p[0];
   		pixel->channel[A] = p[3];
	} 
	else if (bytes == 3) {
		pixel->channel[R] = p[2];
		pixel->channel[G] = p[1];
		pixel->channel[B] = p[0];
  
	} 
	else if (bytes == 2) {
		pixel->channel[R] = (p[1] & 0x7c) << 1;
    		pixel->channel[G] = ((p[1] & 0x03) << 6) | ((p[0] & 0xe0) >> 2);
   		pixel->channel[B] = (p[0] & 0x1f) << 3;
   		pixel->channel[A] = (p[1] & 0x80);
   }
}



bool TGA::save(const std::string& fileName, Header * H, Pixel * P) {
		
	std::cout << "Saving " << "\"" << fileName.c_str() << "\"" << std::endl;

	std::ofstream tgaFile(fileName, std::ofstream::binary);

	if (!tgaFile.is_open()) {
		std::cerr << "Error opening" << fileName.c_str() << std::endl;
		return false;
	}

	tgaFile.write (&H->idlength, sizeof(H->idlength));
	tgaFile.write (&H->colourmaptype, sizeof(H->colourmaptype));
	tgaFile.write (&H->datatypecode, sizeof(H->datatypecode));

	
	tgaFile.write ((char*) &H->colourmaporigin, sizeof(H->colourmaporigin));
	tgaFile.write ((char*) &H->colourmaplength, sizeof(H->colourmaplength));
	tgaFile.write (&H->colourmapdepth, sizeof(H->colourmapdepth));

	tgaFile.write ((char*) &H->x_origin, sizeof(H->x_origin));
	tgaFile.write ((char*) &H->y_origin, sizeof(H->y_origin));
	tgaFile.write ((char*) &H->width, sizeof(H->width));
	tgaFile.write ((char*) &H->height, sizeof(H->height));
	tgaFile.write (&H->bitsperpixel, sizeof(H->bitsperpixel));
	tgaFile.write (&H->descriptor, sizeof(H->descriptor));

	for (int i=0; i < H->height * H->width; i++) {
		tgaFile.write((char*)&P[i].channel[B], 1);
		tgaFile.write((char*)&P[i].channel[G], 1);
		tgaFile.write((char*)&P[i].channel[R], 1);
		if (H->hasalpha) {
			tgaFile.write((char*)&P[i].channel[A], 1);
		}
	}

	delete [] P;

	return true; 
}



bool TGA::halfsize(const std::string& fileName) {

	int half_height = (int)(header.height / 2);
   	int half_width = (int)(header.width / 2);
	Pixel * P = new Pixel [half_height* half_width];
	int xi, yi, xf, yf;

	std::cout << "Original size: " << header.height << "x" << header.width << std::endl;
	std::cout << "Resizing to: " << half_height << "x" << half_width << std::endl;

	for (int i = 0; i < half_width; i++) {
		for (int j =0; j < half_height; j++) {
			xi = 2 * i;
			yi = 2 *j;

			xf = std::min(xi + 1, header.width - 1);
			yf = std::min(yi + 1, header.height - 1);
	
			//iterate over the channels 
			for (int c = 0; c < 3; c++) {
				//interpolate			
			  	P[j * half_width + i].channel[c] = (int) (pixeldata[yi * header.width + xi].channel[c] + 0.5);
			}		
		}
	}


	std::cout << "Done." << std::endl;

	Header newheader = header; 
	newheader.width = half_width;
	newheader.height = half_height;

	return save(fileName, &newheader, P); 
	
}


