#include "TGA.h"

int main(int argc,char *argv[]) {

	if (argc < 2) {
		std::cout <<"Usage: " << argv[0] << " tgafile" << std::endl;
      	exit(1);
	}

	TGA image; 
	image.load(argv[1]);
	image.halfsize(argv[2]);

	return 0;


}
