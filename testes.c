#include "frame.h"

void main(){
	Frame frame;
	
	frameSetup(&frame,"TESTE", 12, 10,'n');
	frameInsert(&frame, "escrevendo normal...", 1,2);
	frameInsertCenter(&frame,"(centralizado)", 3);
	
	frameInsertOption(&frame,"opcao 1");
	frameCleanOption(&frame);
	frameInsertOption(&frame,"opcao 2");
	frameInsertOption(&frame,"opcao 3");
	
	imageShow(&frame.image);
	
	char buffer[2];
	
	while(1){
		
		fgets(buffer,3,stdin);
		setbuf(stdin,NULL);
		
		if(buffer[0] != NULL)
			printf("char: %c\n", buffer[0]);
		
		if(buffer[0] == 'w'){
			frameSetCursor(&frame,2,frame.cursor[1] - 1);
			imageShow(&frame.image);
		}
		else if(buffer[0] == 's'){
			frameSetCursor(&frame,2,frame.cursor[1] + 1);
			imageShow(&frame.image);
		}
			
	
	}
}
