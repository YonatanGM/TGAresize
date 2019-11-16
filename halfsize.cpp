#include "TGA.h"

int main(int argc,char *argv[]) {

	if (argc < 2) {
		std::cout <<"Usage: " << argv[0] << " tgafile" << std::endl;
      	exit(1);
	}


	TGA image; 
    	if (!image.load(argv[1])) exit(1);

	if(!image.halfsize(argv[2])) exit(1);
	

	return 0;


}
