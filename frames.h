#include <stdlib.h>
#include <string.h>

/************************/
/*     ESTRUTURA        */
/************************/
typedef struct{
	int width, height;
	char *image;
	char *title;
	char border;
}frame;

typedef struct{
	int width, height;
	char *image;
	char *titles;
	int qtdCol;
	int *colWidth;

}table;

/************************/
/*   PROTOTIPAGEM       */
/************************/
table constructorTable(table *Table,char *subTitles);

frame frameSetup(char *title,char border ,int width, int height){
	frame Frame;
	Frame.width = width+2;
	Frame.height = height+2;

	int sizeTitle = strlen(title)+1;

	Frame.image = (char *)malloc(Frame.width*Frame.height);
	Frame.title = (char *)malloc(sizeTitle);

	strcpy(Frame.title,title);
	Frame.border = border;

	return Frame;
}

//_____________//

table tableSetup(table Table, char *subTitles,char *colWidth){
	//"50,10,30"
	int sizeSubTitles = 0;
	int sizeColBuffer = 0;
	
	int sizeText = strlen(colWidth), cursor = 0,qtdCol = 1;
	char *buffer = (char *)malloc(sizeText);
	
	Table.width = 0;
	Table.height = 2;

	sizeSubTitles = strlen(subTitles);

	for(int i = 0;i < sizeSubTitles;i++){
		if(subTitles[i] == ',')
			qtdCol++;
	}
	
	Table.colWidth = (int *)malloc(qtdCol);
	Table.qtdCol = qtdCol;

	for(int i = 0;i < Table.qtdCol;i++){
		for(int j = 0;j < sizeText;j++){
			if(colWidth[cursor] != ','){
				buffer[j] = colWidth[cursor];
				cursor++;
			}
			else{
				buffer[j] = '\0';
				Table.colWidth[i] = atoi(buffer) +1;
				Table.width += Table.colWidth[i];
				printf("tamanho coluna: %d \n",Table.colWidth[i]);
				cursor++;
				break;
			}
			
		}
	}
	
	Table  = constructorTable(&Table, subTitles);

	return Table;
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
			Frame.image[j] = Frame.title[cursor];
			cursor++;
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

//_____________//



table refreshCol(table Table,int col,char *type,void *values, int qtdValues){
	//Limpa todos os valores da coluna
	//Escreve os valores passados

	//Redimensiona a tabela se necessário

	//numeros[5] = {1,2,3,4,5}
	//tabela = refreshCol(tabela,'i',numeros,5)
	
	int i = 0,j = 0,cursor = (Table.width*2) +Table.colWidth[col];
	char *buffer = (char *)malloc(Table.colWidth[col]);
	
	if(Table.height < qtdValues+2)
		Table.height = qtdValues+2;
	
	Table.image = (char *)realloc(Table.image,Table.width*Table.height);
	
	//Limpar Tabela e adiciona valores
	for(i = 2;i < Table.height;i++){
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
		
		for(j = 0;j < Table.colWidth[col]-1;j++){
			Table.image[cursor++] = ' ';
		}
		
		for(j = Table.colWidth[col]-1;j > -1;j--){
			Table.image[cursor--] = buffer[j];
		}
	}
			
	return Table;	
}


/************************/
/*   BANCO DE LAYOUTS   */
/*                      */
/*(Provisório p/ testes)*/
/************************/
frame frames(int frameID){
	frame Frame = frameSetup("NONE",'|',1,1);

	switch(frameID){
		case 0:
			Frame = frameSetup("POP UP",'!',10,3);
			constructorWindow(Frame);

			Frame = frameResize(&Frame,20,4);
			break;
		case 1:
			Frame = frameSetup("JANELA",'|',20,4);
			constructorWindow(Frame);

			table Table;
			Table = tableSetup(Table,"Nome,Idade,Endereco,CEP","15,5,10,5");

			Frame = frameResize(&Frame,10,5);
			
			
			int test[] = {5,1,3,4,8};
			refreshCol(Table,0,"%d",test, 5);

			frameInsertTable(&Frame,Table,0,0);
			frameInsertTable(&Frame,Table,Table.width,0);
			break;
	}
	return  Frame;
}
