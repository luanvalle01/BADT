#include "frame.h"

void main(){
	Frame frame;
	frameSetup(&frame,"TESTE", 12, 10,'n');

	imageInsert(&frame.image,"Escrita...",10,5);

	frameResize(&frame, 50,20);

	imageShow(&frame.image);
}
