#include "frame.h"

void mainMenu(Frame *frame){

	frameSetup(frame,"menu", 50, 10,'n');
	frameInsert(frame, "escrevendo normal...", 1,2);
	frameInsertCenter(frame,"(centralizado)", 3);
	
	
	frameInsertOption(frame,"opcao 1");
	frameCleanOption(frame);
	frameInsertOption(frame,"ver tabela");
	frameInsertOption(frame,"ver input");
	frameInsertOption(frame,"Sair");
	
	
	frameInsert(&frame[0], "W - cima", 1,9);
	frameInsert(&frame[0], "D - baixo", 1,10);
}

void main(){
	int state = 0, run = 1;
	
	Frame frame[3];
	
	mainMenu(&frame[0]);
	Table table;
	tableSetup(&table, "dscif","id,nome,letra,numero,dinheiro");
	txtToTable(&table,"./teste");
	
	frameSetup(&frame[1],"tabela", 5, 20,'n');
	frameSetTable(&frame[1], &table);
	frameRefresh(&frame[1]);
	
	frameSetup(&frame[2],"input", 10, 5,'n');
	frameSetInput(&frame[2], "Digite seu CPF (apenas numeros)", 10);
	
	char buffer = '\0';
	float testeSOMA = 10;
	
	while(run){		
	
		frame[state].inputBuffer[0] = buffer;
		frameRefresh(&frame[state]);
		frameShowOption(&frame[0]);
		
		clear();
		imageShow(&frame[state].image);
		
      	if(buffer != 10)
			buffer = getch();
     	
     	if(!state){
     		
	        if(buffer == 10){
				if(frame[0].optionSelect == 0)
					state = 1;
				if(frame[0].optionSelect == 1)
					state = 2;
				if(frame[0].optionSelect == 2)
					run = false;

				buffer = '\0';
			}

		}
		else if(state == 1){
			if(buffer == 'd' || buffer == 'D'){
				testeSOMA += .10;
				tableUpdate(&table, 4, frame[1].table->actualPage, &testeSOMA);
			}

			if(buffer == 10){
				state = 0;
				buffer = '\0';
			}
		}
		else if(state == 2){
			if(buffer == 10){
				state = 0;
				buffer = '\0';
			}
		}
	}

	clear();
	
	tableToTxt(&table,"./teste");
	
	frameFree(&frame[0]);
	frameFree(&frame[1]);
	tableFree(&table);
	frameFree(&frame[2]);
	
	printf("====================================\n");
	printf("|                                  |\n");
	printf("| Programa encerrado corretamente! |\n");
	printf("|                                  |\n");
	printf("====================================\n");

}
