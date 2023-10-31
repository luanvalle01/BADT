#include <stdlib.h>
#include <string.h>
#include "suportFunctions.h"

/************************/
/*     ESTRUTURA        */
/************************/
typedef struct{
	int width, height;
	char *image;
	char *titles;
	int qtdCol;
	int *colWidth;

}table;

typedef struct{
	int width, height;
	char *image;
	char *title;
	char border;
	
	table Table;
}frame;

/************************/
/*   PROTOTIPAGEM       */
/************************/
//Setup
frame frameSetup(char *title,char border ,int width, int height);
void tableSetup(table *Table, char *subTitles,char *colWidth);

//Construtores
void constructorLine(frame Frame, int line, char *text);
void constructorWindow(frame Frame);
void freeFrame(frame Frame);

void constructorTable(table *Table,char *subTitles);
void freeTable(table Table);

//Editores
frame frameCopy(frame frameOrigin);
frame frameResize(frame *Frame,int newWidth,int newHeight);
void frameInsert(frame *Frame,char *text,int x,int y);
void frameInsertTable(frame *Frame,table Table,int x,int y);
void frameRefresh(frame *Frame);

void refreshCol(table *Table,int col,char type,void *values, int qtdValues);
void refreshTable(table *Table);

frame frameSetup(char *title,char border ,int width, int height){
	frame Frame;
	Frame.width = width+2;
	Frame.height = height+2;

	int sizeTitle = strlen(title)+1;

	Frame.image = (char *)malloc(Frame.width*Frame.height);
	Frame.title = (char *)malloc(sizeTitle);

	strcpy(Frame.title,title);
	Frame.border = border;

	Frame.Table.image = (char *)malloc(1);
	Frame.Table.image[0] = '\0';

	constructorWindow(Frame);

	return Frame;
}

//_____________//

void tableSetup(table *Table, char *subTitles,char *colWidth){
	int count = 0;
	int sizeSubTitles = 0;
	int sizeColBuffer = 0;
	
	int sizeText = strlen(colWidth), cursor = 0,qtdCol = 2;
	char *buffer = (char *)malloc(sizeText);
	
	Table->width = 0;
	Table->height = 2;

	sizeSubTitles = strlen(subTitles)+1;
	Table->titles = (char *)malloc(sizeSubTitles);

	for(count = 0;count < sizeSubTitles;count++){
		Table->titles[count] = subTitles[count];
		if(Table->titles[count] == ',')
			qtdCol++;
	}
	
	Table->titles[count+1] = '\0';
	
	Table->colWidth = (int *)malloc(qtdCol);
	Table->qtdCol = qtdCol;

	for(int i = 0;i < Table->qtdCol;i++){
		for(int j = 0;j < sizeText;j++){
			if(colWidth[cursor] != ','){
				buffer[j] = colWidth[cursor];
				cursor++;
			}
			else{
				buffer[j] = '\0';
				Table->colWidth[i] = atoi(buffer) +1;
				Table->width += Table->colWidth[i];
				printf("tamanho coluna: %d \n",Table->colWidth[i]);

				cursor++;
				break;
			}
			
		}
	}
	
	constructorTable(Table, subTitles);

}

/************************/
/*    CONSTRUTORES      */
/************************/
void constructorLine(frame Frame, int line, char *text){
	int pos = line*Frame.width;
	for(int i = 0; i < Frame.width; i++){
		Frame.image[pos] = text[i];
		pos++;
	}
}

void constructorWindow(frame Frame){
	int cursor = 0,titleSize;
	float xTitle, xTitleFinal;
	int i = 0, j = 0;

	titleSize = strlen(Frame.title)+1;
	if(titleSize > Frame.width - 1){
		printf("title oversized!");
		return;
	}

	xTitle = (Frame.width/2) - (titleSize/2);
	xTitleFinal = xTitle + titleSize-1;

	for(j; j < Frame.width; j++){
		Frame.image[j] = '=';
		if(j >= xTitle &&  j < xTitleFinal){

			Frame.image[j] = Frame.title[cursor++];
		}
	}

	cursor = Frame.width;

	for(i = 1; i < Frame.height; i++){
		for(j = 0; j < Frame.width; j++){
			if(i == Frame.height - 1)
				Frame.image[cursor] = '-';
			else if(j == 0 || j == Frame.width -1)
				Frame.image[cursor] = Frame.border;
			else
				Frame.image[cursor] = ' ';
			cursor++;
		}
	}
}

void freeFrame(frame Frame){
	free(Frame.image);
	free(Frame.title);
}

//_____________//

void constructorTable(table *Table,char *subTitles){
	//Cria uma tabela com os títulos das colunas já inseridos
	//"Nome,Idade,Estado"

	//A altura inicial é 0(sem contar sub títulos);
	int cursor = 0, cursorSubTitles = 0, sizeColBuffer = 0, emptySize = 0, i = 0, j = 0;
	int subTitlesSize = strlen(subTitles);
	int imageSize = (Table->width * Table->height) +Table->qtdCol;
	Table->image = (char *)realloc(Table->image,imageSize);
	
	for(i = 0; i < Table->qtdCol; i++){
		sizeColBuffer = 0;
	
		for(j = 0; j < subTitlesSize; j++){
			if(subTitles[cursorSubTitles++] != ',' && subTitles[cursorSubTitles])
				sizeColBuffer++;
			else 
				break;
		}
		if(sizeColBuffer > Table->colWidth[i]){
			Table->colWidth[i] = sizeColBuffer +1;
			sizeColBuffer = 0;
			refreshTable(Table);
			return;
		}
	}

	cursorSubTitles = 0;

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

	//Frame = frameResize(&Frame,50,5);

	for(i = 0; i < textSize;i++){
		if(Frame->image[cursor] != Frame->border)
			Frame->image[cursor++] = text[i];
	}
}

void frameInsertTable(frame *Frame,table Table,int x,int y){
	int i = 0,j = 0, cursorTable = 0;
	char *buffer = (char *)malloc(Table.width);

	*Frame = frameResize(Frame,x + Table.width,5);

	for(i; i < Table.height; i++){
		for(j = 0; j < Table.width; j++){
			buffer[j] = Table.image[cursorTable++];
		}
		frameInsert(Frame, buffer, x, y++);
	}

	free(buffer);
}

void frameRefresh(frame *Frame){
	if(Frame->Table.image[0] != '\0')
		frameInsertTable(Frame,Frame->Table,0,0);
	//Acrescentar Rodapé
}

//_____________//

void refreshCol(table *Table,int col,char type,void *values, int qtdValues){
	//Limpa todos os valores da coluna
	//Escreve os valores passados

	//Redimensiona a tabela se necessário

	//numeros[5] = {1,2,3,4,5}
	//tabela = refreshCol(tabela,'i',numeros,5)
	
	int i = 0,j = 0,cursor = (Table->width*2), sizeBuffer = 0;
	char *buffer = (char *)malloc(Table->colWidth[col]);
	char *tempString = (char *)values;
	int stop = 0;
	
	sizeBuffer = sizeofStr(tempString,'|');
	
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
		if(strcmp(type,"%d") == 0 || strcmp(type,"%i") == 0){
			int *temp = (int *)values;
			sprintf(buffer,"%d",temp[0]);
		}
		else if(strcmp(type,"%f") == 0){
			float *temp = (float *)values;
			sprintf(buffer,"%.2f",temp[i-2]);
		}
		else if(strcmp(type,"%s") == 0){
			char **temp = (char **)values;
			strcpy(buffer,*temp[i-2]);
		}
		
		Table->image[cursor++] = ' ';
		stop = 0;
		
		for(j = 0;j < Table->colWidth[col]-2;j++){
			Table->image[cursor] = ' ';
			if(buffer[j] && !stop)
				Table->image[cursor] = buffer[j];
			else
				stop = 1;
			
			cursor++;
		}
		if(col < Table->qtdCol-1)
			Table->image[cursor] = ':';
		cursor++;
		
		cursor += Table->width - Table->colWidth[col];
	}

}

void refreshTable(table *Table){
	int i = 0;
	char *buffer = (char *)malloc(100);
	char *colWidths = (char *)malloc(100);
	
	
	for(i = 0; i < Table->qtdCol; i++){
		sprintf(buffer,"%d",Table->colWidth[i]);
		
		if(!i){
			strcpy(colWidths,buffer);
			strcat(colWidths,",");
		}
		else{
			strcat(colWidths,buffer);
			if(i < Table->qtdCol-1)
		 		strcat(colWidths,",");
		 	else
		 		strcat(colWidths,"\0");
		}
		
		
	}
	
	colWidths = (char *)realloc(colWidths,strlen(colWidths));
	printf("%s\n",colWidths);
	printf("%s\n",Table->titles);
	tableSetup(Table,Table->titles,colWidths);
	
	free(buffer);
}


/************************/
/*   BANCO DE LAYOUTS   */
/*                      */
/*(Provisório p/ testes)*/
/************************/
frame frames(int frameID){
	frame Frame = frameSetup("TITULO",'|',10,5);

	switch(frameID){
		case 0:
			Frame = frameSetup("POP UP",'!',20,3);
			
			frameInsert(&Frame,"Testando pop up",Frame.width/2-9,Frame.height/2-1);
			frameRefresh(&Frame);
			break;
		case 1:

			tableSetup(&Frame.Table,"Nome,Idade,Endereco,Dinheiros","15,1,10,15");
			//Frame = frameResize(&Frame,10,5);
			
			char nomes[5][50];
			//cleanStr((char *)nomes,' ',5*50);
			strcpy(nomes[0],"|Pablo Fernando\0");
			strcpy(nomes[1],"|Roberto Carlos\0");
			strcpy(nomes[2],"|Jacinto Pinto\0");
			strcpy(nomes[3],"|Tomas Turbando\0");
			strcpy(nomes[4],"|Paula Tejando\0");
			
			int idades[] = {18,10,34,39,26};
			
			char enderecos[5][50];
			//cleanStr((char *)enderecos,' ',5*50);
			strcpy(enderecos[0],"|Rua das Casas, 123\0");
			strcpy(enderecos[1],"|Rua dos Moradores, 321\0");
			strcpy(enderecos[2],"|Beco do Batman, 100\0");
			strcpy(enderecos[3],"|Casa da Mae Joana, 5\0");
			strcpy(enderecos[4],"|Beto Carreiro, 97123\0");
			
			float dinheiros[] = {99.10,10.15,1234.71,39,0};
			
			Frame.Table.colWidth[0] = biggestStr(nomes,'s',5);
			Frame.Table.colWidth[1] = biggestStr(idades,'d',5);
			Frame.Table.colWidth[2] = biggestStr(enderecos,'s',5);
			Frame.Table.colWidth[3] = biggestStr(dinheiros,'f',5);
			
			refreshTable(&Frame.Table);
			
			refreshCol(&Frame.Table,0,'s',nomes, 5);
			refreshCol(&Frame.Table,1,'d',idades, 5);
			refreshCol(&Frame.Table,2,'s',enderecos, 5);
			refreshCol(&Frame.Table,3,'f',dinheiros, 5);

			frameRefresh(&Frame);
			break;
	}
	return  Frame;
}
