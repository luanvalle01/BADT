#include "suportFunctions.h"
#include <stdlib.h>

typedef struct{
	int width, height;
	char *image;
}frame;


void constructorAll(frame Frame,char *image){
	int pos = 0;
	for(int i = 0; i < Frame.height; i++){
		for(int j = 0; j < Frame.width; j++){
			Frame.image[pos] = image[pos];
			pos++;
		}
	}
}

void constructorLine(frame Frame, int line, char *text){
	int pos = line*Frame.width;
	for(int i = 0; i < Frame.width; i++){
		Frame.image[pos] = text[i];
		pos++;
	}
}

frame frameSetSize(int width, int height){
	frame Frame;
	Frame.width = width;
	Frame.height = height;

	Frame.image = (char *)malloc(width*height);

	return Frame;
}

void freeFrame(frame Frame){
	free(Frame.image);
	free(Frame.title);
}

//_____________//

table constructorTable(table *Table,char *subTitles){
	//Cria uma tabela com os títulos das colunas já inseridos
	//"Nome,Idade,Estado"

	//A altura inicial é 0(sem contar sub títulos);
	int cursor = 0, cursorSubTitles = 0, sizeColBuffer = 0, emptySize = 0, i = 0, j = 0;

	int imageSize = (Table->width * Table->height) +Table->qtdCol;
	Table->image = (char *)malloc(imageSize);

	for(i = 0;i < Table->qtdCol;i++){
		for(j = 0;j < Table->colWidth[i]; j++){
			if(subTitles[cursorSubTitles] == ',')
				Table->image[cursor++] = ' ';
			else
				Table->image[cursor++] = subTitles[cursorSubTitles++];
		}
		cursor--;
		Table->image[cursor++] = ':';
		cursorSubTitles++;
	}
	
	for(int i = 0;i < Table->width;i++)
		Table->image[cursor++] = '-';

	return *Table;
}

void freeTable(table Table){
	free(Table.image);
}

/************************/
/*      EDITORES        */
/************************/
frame frameCopy(frame frameOrigin){
	int cursor = 0;
	frame frameDestiny;
	frameDestiny = frameSetup(frameOrigin.title,frameOrigin.border,frameOrigin.width,frameOrigin.height);

	frameDestiny.image = (char *)malloc(frameOrigin.width*frameOrigin.height);

	for(int i = 0;i < frameOrigin.height;i++){
		for(int j = 0; j < frameOrigin.width;j++){
			frameDestiny.image[cursor] = frameOrigin.image[cursor];
			cursor++;
		}
	}

	return frameDestiny;
}

frame frameResize(frame *Frame,int newWidth,int newHeight){
	int cursor = Frame->width-2,cursorBuffer = newWidth,i = 1,j = 1;

	frame frameBuffer = frameSetup(Frame->title,Frame->border,newWidth,newHeight);
	constructorWindow(frameBuffer);

	for(i = 1; i < frameBuffer.height - 1; i++){
		for(j = 0; j < frameBuffer.width; j++){

			if(frameBuffer.image[cursorBuffer] == frameBuffer.border){
				cursorBuffer++;
			}

			if(Frame->image[cursor] == Frame->border){
				cursor++;
			}

			if(cursor > (Frame->width * Frame->height)-Frame->width-1 || cursorBuffer > newWidth * newHeight)
				break;

			if(Frame->image[cursor] != Frame->border && frameBuffer.image[cursorBuffer] != frameBuffer.border)
				frameBuffer.image[cursorBuffer++] = Frame->image[cursor++];

			//Pula a linha do Buffer caso não exista mais dados na origem
			if(j == Frame->width-2){
				while(j < frameBuffer.width-2){
					j++;
					cursorBuffer++;
				}
			}

			//Pula a linha da origem caso não exista mais espaço no buffer
			if(j == newWidth-2 && newWidth < Frame->width){
				int count = 0;

				while(count < Frame->width - newWidth){
					count++;
					cursor++;
				}
			}
		}
	}

	//freeFrame(frameBuffer);

	return frameBuffer;
}

void frameInsert(frame *Frame,char *text,int x,int y){
	//Escreve o texto na coordenada especifico
	int i = 0,cursor = Frame->width+2;
	int textSize = strlen(text);
	cursor += (((y)*Frame->width)-1)+x;


	for(i = 0; i < textSize;i++){
			Frame->image[cursor++] = text[i];
	}
}

void frameInsertTable(frame *Frame,table Table,int x,int y){
	int i = 0,j = 0, cursorTable = 0;
	char *buffer = (char *)malloc(Table.width);

	*Frame = frameResize(Frame,x + Table.width,y+Table.height);

	for(i; i < Table.height; i++){
		for(j = 0; j < Table.width; j++){
			buffer[j] = Table.image[cursorTable++];
		}
		frameInsert(Frame, buffer, x, y++);
	}

	free(buffer);
}

//_____________//

table refreshCol(table *Table,int col,char type,void *values, int qtdValues){
	//Limpa todos os valores da coluna
	//Escreve os valores passados

	//Redimensiona a tabela se necessário

	//numeros[5] = {1,2,3,4,5}
	//tabela = refreshCol(tabela,'i',numeros,5)
	
	int i = 0,j = 0,cursor = (Table->width*2), sizeBuffer = 0;
	char *buffer = (char *)malloc(Table->colWidth[col]);
	char *tempString = (char *)values;
	
	sizeBuffer = sizeofStr(tempString);
	
	printf("TAMANHIOO: %d",sizeBuffer);
	
	for(i = 0; i < col; i++)
		cursor +=  Table->colWidth[i];
	
	for(i = 0;i < Table->colWidth[col];i++)
		buffer[i] = ' ';
		
	buffer[i-1] = '\0';
	
	
	if(Table->height < qtdValues+2)
		Table->height = qtdValues+2;
	
	Table->image = (char *)realloc(Table->image,Table->width*Table->height);
	
	
	
	
	//Limpar Tabela e adiciona valores
	for(i = 2;i < Table->height;i++){
		if(type == 'd'|| type == 'i'){
			int *temp = (int *)values;
			sprintf(buffer,"%d",temp[i-2]);
		}
		else if(type == 'f'){
			float *temp = (float *)values;
			sprintf(buffer,"%.2f",temp[i-2]);
		}
		if(type == 's'){
			for(j = 0;j < Table->colWidth[col]; j++){
					buffer[j] = tempString[((i-2) * sizeBuffer) +j+1];
			}
		}
		
		Table->image[cursor++] = ' ';
		
		for(j = 0;j < Table->colWidth[col]-2;j++){
			Table->image[cursor] = ' ';
			if(buffer[j])
				Table->image[cursor] = buffer[j];
			
			cursor++;
		}
		Table->image[cursor++] = ':';
		cursor += Table->width - Table->colWidth[col];
	}
			
	return *Table;	
}


/************************/
/*   BANCO DE LAYOUTS   */
/*                      */
/*(Provisório p/ testes)*/
/************************/

frame frames(int frameID){
	frame Frame = frameSetSize(1,1);

	switch(frameID){
		case 0:
			Frame = frameSetup("POP UP",'!',20,3);
			constructorWindow(Frame);
			
			frameInsert(&Frame,"Testando pop up",Frame.width/2-9,Frame.height/2-1);
			break;
			
		case 1:
			table Table;
			Table = tableSetup(Table,"Nome,Idade,Endereco,Dinheiros","15,10,10,5");

			Frame = frameResize(&Frame,10,5);
			
			char nomes[5][50];
			//cleanStr((char *)nomes,' ',5*50);
			strcpy(nomes[0],"|Pablo Fernando");
			strcpy(nomes[1],"|Roberto Carlos");
			strcpy(nomes[2],"|Jacinto Pinto");
			strcpy(nomes[3],"|Tomas Turbando");
			strcpy(nomes[4],"|Paula Tejando");
			
			int idades[] = {18,10,34,39,26};
			
			char enderecos[5][50];
			//cleanStr((char *)enderecos,' ',5*50);
			strcpy(enderecos[0],"|Rua das Casas, 123");
			strcpy(enderecos[1],"|Rua dos Moradores, 321");
			strcpy(enderecos[2],"|Beco do Batman, 100");
			strcpy(enderecos[3],"|Casa da Mae Joana, 5");
			strcpy(enderecos[4],"|Beto Carreiro, 97123");
			
			float dinheiros[] = {99.10,10.15,123.71,39,0};
				
			
			Table = refreshCol(&Table,0,'s',nomes, 5);
			Table = refreshCol(&Table,1,'d',idades, 5);
			Table = refreshCol(&Table,2,'s',enderecos, 5);
			Table = refreshCol(&Table,3,'f',dinheiros, 5);

			frameInsertTable(&Frame,Table,0,0);
			break;
	}
	
	return Frame;
}
