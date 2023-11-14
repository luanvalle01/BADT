#include "frame.h"

void insertOption(Frame *frame){
	char optionS[30];
	sprintf(optionS, "Opcao selecionada: %i\0", frame->optionSelect);
        optionS[29] = '\0';
        frameInsertCenter(frame,optionS, 3);	
}

void main(){
	Frame frame;
	
	frameSetup(&frame,"TESTE", 50, 10,'n');
	frameInsert(&frame, "escrevendo normal...", 1,2);
	frameInsertCenter(&frame,"(centralizado)", 3);
	
	frameInsertOption(&frame,"opcao 1");
	frameCleanOption(&frame);
	frameInsertOption(&frame,"opcao 2");
	frameInsertOption(&frame,"opcao 3");
	frameInsertOption(&frame,"opcao opcional");
	
	Table table;
	tableSetup(&table, "s,d,i,f","nome,idade,numero,dinheiro");
	
	int valor[1];
	valor[0] = 10; 
	tableInsert(&table, 0, 0, valor, 'i');
	
	printf("%s",table.registry);
	
	/*
	clear();
	imageShow(&frame.image);
	
	char buffer;
	
	while(1){
		
        buffer = getch();
        
        if(buffer == 10){	
        	frameInsert(&frame, "W - cima", 1,9);
			frameInsert(&frame, "D - baixo", 1,10);
		}
		
		if(buffer == 'w'){
			clear();
			frameCursorSub(&frame);
			insertOption(&frame);
			imageShow(&frame.image);
		}
		else if(buffer == 's'){
			clear();
			frameCursorAdd(&frame);
			insertOption(&frame);
			imageShow(&frame.image);
		}		
	}
	*/
}
