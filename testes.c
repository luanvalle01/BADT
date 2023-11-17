#include "frame.h"

void insertOption(Frame *frame){
	char optionS[30];
	sprintf(optionS, "Opcao selecionada: %i", frame->optionSelect);
        optionS[29] = '\0';
        frameInsertCenter(frame,optionS, 3);	
}

void mainMenu(Frame *frame){

	frameSetup(frame,"MENU", 50, 10,'n');
	frameInsert(frame, "escrevendo normal...", 1,2);
	frameInsertCenter(frame,"(centralizado)", 3);
	
	frameInsertOption(frame,"opcao 1");
	frameCleanOption(frame);
	frameInsertOption(frame,"opcao 2");
	frameInsertOption(frame,"opcao 3");
	frameInsertOption(frame,"opcao opcional");
	
	frameInsert(&frame[0], "W - cima", 1,9);
	frameInsert(&frame[0], "D - baixo", 1,10);
}

void table(Frame * frame){

	
}

void main(){
	int state = 0;
	Frame frame[2];
	
	mainMenu(&frame[0]);
	Table table;
	tableSetup(&table, "sdif","nome,idade,numero,dinheiro");
	txtToTable(&table,"./teste");
	
	frameSetup(&frame[1],"TABELA", 50, 10,'n');
	frameSetTable(&frame[1], &table);
	frameRefresh(&frame[1]);
	
	clear();
	imageShow(&frame[0].image);
	
	char buffer = '\0';
	
	while(1){
		clear();
		imageShow(&frame[state].image);
		
        if(buffer != 10)
			buffer = getch();
        
     	
     	if(!state){
	        if(buffer == 10){
			
				state = 1;
			
				buffer = '\0';
			}
			
			if(buffer == 'w' || buffer == 'W'){
				clear();
				frameCursorSub(&frame[0]);
				insertOption(&frame[0]);
				imageShow(&frame[0].image);
			}
			else if(buffer == 's' || buffer == 'S'){
				clear();
				frameCursorAdd(&frame[0]);
				insertOption(&frame[0]);
				imageShow(&frame[0].image);
			}
		}
		else{
			 if(buffer == 10){
				state = 0;
				buffer = '\0';
			}
		}
	}
}
