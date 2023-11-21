#ifndef TABLE_H
#define TABLE_H

/******************************************************************************/
/* DEESENVOLVIDO POR LUAN VALLE                                               */
/* https://github.com/luanvalle01/BADT                                        */
/* (V 1.0)                                                                    */
/*                                                                            */
/*  A estrutura >table< e seus métodos foram criados para manipular arquivos  */
/* intuitivamente e não destrutiva, sua utilização depende unicamente de      */
/* str.h                                                                      */
/*                                                                            */
/******************************************************************************/

#include "str.h"

typedef struct{
	char *directory;

	char *types;
	char *colTitles;
	int qtdLine;
	int qtdCol;
	char *registry;

	int *bigOfCol;
	int actualPage;
}Table;

//Constructor
void tableSetup(Table *table, char *types, char *colTitles);

//Suport
void tableNewLine(Table *table);
int tableCursor(Table *table, int col, int line);
void tableBuffer(Table *table, int initialCursor, char *buffer);

//Editors
void tableRealloc(Table *table, int newSize);
void tableCopy(Table *destiny, Table *origin);

void tableInsert(Table *table, int col, int line, void *value);
void tableInsertT(Table *table, int col, int line, void *value, char type);
void tableInsertS(Table *table, int col, int line, char *value);

void tableDelete(Table *table, int col, int line);
void tableUpdate(Table *table, int col, int line, void *value);

//Handlers
void tableToTxt(Table *table,char *directory);
void txtToTable(Table *table, char *directory);
void tableRefresh(Table *table);

//Output
void tableGet(Table *table, int col, int line, char *out);

//Free mallocs
void tableFree(Table *table);

//Legacy
int tableGetJump(Table *table, int linesVisible);

//-------//

void tableSetup(Table *table, char *types, char *colTitles){

	int lenTypes = strLen(types);
	int lenColTitles = strLen(colTitles);

	table->directory = (char *)malloc(2);
	table->directory[0] ='\0';

	table->types = (char *)malloc(lenTypes);
	strCopy(table->types, types);

	table->colTitles = (char *)malloc(lenColTitles);
	strCopy(table->colTitles, colTitles);

	table->qtdLine = 0;

	table->qtdCol = lenTypes;
	table->registry = (char *)malloc(table->qtdCol);
	table->registry[0] = '\0';

	table->bigOfCol = (int *)malloc(table->qtdCol * 4);

	for(int i = 0; i < table->qtdCol; i++){
		if(i != table->qtdCol - 1)
			lenColTitles = strLenPlus(colTitles,',');
		else
			lenColTitles = strLen(colTitles);

		table->bigOfCol[i] = lenColTitles;
		colTitles += lenColTitles + 1;
	}

	table->actualPage = 0;

	tableNewLine(table);
}

void tableNewLine(Table *table){

	int lenTable = strLen(table->registry);
	int i = 0;
	int max = lenTable + (table->qtdCol * 2);

	tableRealloc(table, max);

	for(i = 0; i < lenTable; i++)
		if(table->registry[i] == '\0')
			table->registry[i] = ' ';

	for(i = lenTable; i < max; i++){
		table->registry[i++] = ' ';
		table->registry[i] = '|';
	}
	i--;
	table->registry[i++] = '\n';
	table->registry[i] = '\0';

	table->qtdLine += 1;
}

int tableCursor(Table *table, int col, int line){

	int colCount = 0, lineCount = 0;
	int i = 0;
	int max = strLen(table->registry) + 1;

	for(i; i < max; i++){
		if(table->registry[i] == '\n'){
			lineCount++;
			i++;
		}

		if(table->registry[i] == '|' && lineCount == line)
			colCount++;

		if(colCount == col && lineCount == line)
			break;
	}
	if(col)
		i++;

	if(i == max)
		return -1;
	else
		return i;
}

void tableBuffer(Table *table, int initialCursor, char *buffer){

	int i = initialCursor;
	int max = strLen(table->registry) + 1;

	for(int j = 0; j < i; j++)
		buffer[j] = ' ';

	while(i < max){
		buffer[i] = table->registry[i];

		if(table->registry[i] != '\n')
			table->registry[i] = ' ';
		i++;
	}
	buffer[i] = '\0';
}

void tableRealloc(Table *table, int newSize){

	int oldSize = strLen(table->registry);

	if(oldSize <= newSize)
		table->registry = (char *)realloc(table->registry, oldSize + (newSize*2));
}

void tableCopy(Table *destiny, Table *origin){

	strCopy(destiny->types, origin->types);
	strCopy(destiny->colTitles, origin->types);
	destiny->qtdLine = origin->qtdLine;
	destiny->qtdCol =  origin->qtdCol;

	tableRealloc(destiny, strLen(origin->registry));
	strCopy(destiny->registry, origin->registry);

	destiny->bigOfCol = origin->bigOfCol;
	destiny->actualPage = origin->actualPage;
}

void tableInsert(Table *table, int col, int line, void *value){

	if(table->types[col] == 's'){
		tableInsertS(table, col, line, (char *)value);
	}
	else{
		tableInsertT(table, col, line, value, table->types[col]);
	}
}

void tableInsertT(Table *table, int col, int line, void *value, char type){

	char *finalValue = (char *)malloc(20);
	char *buffer = (char *)malloc(1);
	int lenValue = 0, lenBuffer = 0;
	int cursor = 0, cursorAux = 0;
	int i = 0;
	int newSize;

	while(line >= table->qtdLine)
		tableNewLine(table);

	if(type == 'i' || type == 'd'){
		int *temp = (int *)value;
		sprintf(finalValue,"%d", *temp);
	}
	else if(type == 'f'){
		float *temp = (float *)value;
		sprintf(finalValue,"%.2f",temp[0]);
	}
	else if(type == 'c'){
		char *temp = (char *)value;
		finalValue[0] = temp[0];
		finalValue[1] = '\0';
	}

	lenValue = strLen(finalValue);
	cursor = tableCursor(table, col, line);
	cursorAux = cursor;

	buffer = (char *)realloc(buffer, strLen(table->registry) + lenValue + 1);
	tableBuffer(table, cursor, buffer);

	for(i; i < lenValue; i++)
		table->registry[cursor++] = finalValue[i];

	newSize = strLen(table->registry) + lenValue;
	tableRealloc(table, newSize);

	i = cursorAux + lenValue;
	lenBuffer = strLen(buffer);

	while(i < newSize){
		table->registry[i] = buffer[i - lenValue+1];
		i++;
	}

	if(table->bigOfCol[col] < lenValue + 1)
		table->bigOfCol[col] = lenValue + 1;

	free(finalValue);
	free(buffer);
}

void tableInsertS(Table *table, int col, int line, char *value){

	char *buffer = (char *)malloc(1);
	int lenValue = 0, lenBuffer = 0;
	int cursor = 0, cursorAux = 0;
	int i = 0;
	int newSize;

	while(line >= table->qtdLine)
		tableNewLine(table);

	lenValue = strLen(value);
	buffer = (char *)realloc(buffer, strLen(table->registry) + lenValue + 1);

	cursor = tableCursor(table, col, line);
	cursorAux = cursor;

	tableBuffer(table, cursor, buffer);

	for(i; i < lenValue; i++)
		table->registry[cursor++] = value[i];

	newSize = strLen(table->registry) + lenValue;
	tableRealloc(table, newSize);

	i = cursorAux + lenValue;
	lenBuffer = strLen(buffer);

	while(i < newSize){
		table->registry[i] = buffer[i - lenValue + 1];
		i++;
	}

	if(table->bigOfCol[col] < lenValue)
		table->bigOfCol[col] = lenValue;

	free(buffer);
}

void tableDelete(Table *table, int col, int line){

	int cursor = tableCursor(table, col, line);
	int lenValue = 0, i = 0;
	int newSize;
	char *buffer = (char *)malloc(strLen(table->registry));

	while(table->registry[cursor] != '|' && table->registry[cursor] != '\n'){
		lenValue++;
		cursor++;
	}

	cursor -= lenValue;
	table->registry[cursor] = ' ';

	tableBuffer(table, cursor + lenValue - 1, buffer);
	newSize = strLen(table->registry) - lenValue + 1;

	cursor++;
	while(cursor < newSize){
		table->registry[cursor] = buffer[cursor + lenValue - 1];
		cursor++;
	}
}

void tableUpdate(Table *table, int col, int line, void *value){

	tableDelete(table, col, line);
	tableInsert(table, col, line, value);
}

void tableToTxt(Table *table,char *directory){

	FILE *file;
	file = fopen(directory, "w");

	int i = 0;

	while(table->registry[i] != '\0')
		fprintf(file, "%c", table->registry[i++]);

	fclose(file);
}

void txtToTable(Table *table, char *directory){

	FILE *file;
	if((file = fopen(directory, "r")) == NULL){
		fclose(file);
		tableToTxt(table, directory);
	}

	int i = 0;
	char buffer= fgetc(file);
	char actualCol = 0;
	int actualLen = 0;
	int directoryLen = strLen(directory);

	if(table->directory[0] == '\0'){
		table->directory = (char *)realloc(table->directory,directoryLen + 1);

		strCopy(table->directory, directory);
	}

	while(buffer != EOF){
		tableRealloc(table, i + 1);
		table->registry[i] = buffer;
		buffer = fgetc(file);

		if(actualLen  + 1 > table->bigOfCol[actualCol])
			table->bigOfCol[actualCol] = actualLen + 1;

		if(buffer == '\n'){
			table->qtdLine += 1;
			actualLen = 0;
			actualCol = 0;
		}
		else if(buffer == '|'){
			actualLen = 0;
			actualCol++;
		}

		actualLen++;
		i++;
	}

	tableRealloc(table, i + 1);
	table->registry[i] = '\0';

	fclose(file);
}

void tableRefresh(Table *table){

	Table tableBuffer;
	tableSetup(&tableBuffer, table->types, table->colTitles);

	tableToTxt(table, table->directory);
	txtToTable(&tableBuffer, table->directory);

	tableFree(&tableBuffer);
}

void tableGet(Table *table, int col, int line, char *out){

	int cursor = tableCursor(table, col, line);
	int i = 0;
	char type = table->types[col];

	while(table->registry[cursor] != '|' && table->registry[cursor] != '\n'){
		out[i] = table->registry[cursor];
		i++;
		cursor++;
	}
	out[i] = '\0';
}

void tableFree(Table *table){

	free(table->types);
	free(table->colTitles);
	free(table->registry);

	free(table->bigOfCol);
}

//-------//

int tableGetJump(Table *table, int initialLine){

	return(tableCursor(table, 0, initialLine));
}

#endif
