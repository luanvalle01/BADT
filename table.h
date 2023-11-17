#ifndef TABLE_H
#define TABLE_H

#include "str.h"

typedef struct{
	char *types;
	char *colTitles;
	int qtdLine;
	int qtdCol;
	char *registry;

	int *bigOfCol;
	int actualPage;
}Table;

void tableSetup(Table *table, char *types, char *colTitles);
void tableRealloc(Table *table, int newSize);

void tableNewLine(Table *table);
int tableCursor(Table *table, int col, int line);
void tableBuffer(Table *table, int initialCursor, char *buffer);

void tableInsert(Table *table, int col, int line, void *value);
void tableInsertT(Table *table, int col, int line, void *value, char type);
void tableInsertS(Table *table, int col, int line, char *value);

void tableDelete(Table *table, int col, int line);
void tableUpdate(Table *table, int col, int line, void *value);

int tableGetJump(Table *table, int linesVisible);

void tableToTxt(Table *table,char *directory);
void txtToTable(Table *table, char *directory);

void tableGet(Table *table, int col, int line, char *out);

//-------//
void tableSetup(Table *table, char *types, char *colTitles){

	int lenTypes = strLen(types);

	table->types = (char *)malloc(lenTypes);
	strCopy(table->types, types);

	table->qtdLine = 0;

	table->qtdCol = lenTypes;
	table->registry = (char *)malloc(table->qtdCol);
	table->registry[0] = '\0';

	table->bigOfCol = (int *)malloc(table->qtdCol * 4);

	for(int i = 0; i < table->qtdCol; i++){
		table->bigOfCol[i] = strLenPlus(colTitles,',');
		colTitles += table->bigOfCol[i - 1] + 1;
	}

	table->actualPage = 0;

	tableNewLine(table);
}


void tableRealloc(Table *table, int newSize){

	int oldSize = strLen(table->registry);

	if(oldSize <= newSize)
		table->registry = (char *)realloc(table->registry, oldSize + (newSize*2));
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

	if(table->bigOfCol[col] < lenValue)
		table->bigOfCol[col] = lenValue;

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
	char *buffer = (char *)malloc(1);

	while(table->registry[cursor] != '|' && table->registry[cursor] != '\n'){
		lenValue++;
		cursor++;
	}

	buffer = (char *)realloc(buffer, strLen(table->registry) + 1);
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

int tableGetJump(Table *table, int initialLine){

	return(tableCursor(table, 0, initialLine));
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

	while(buffer != EOF){
		tableRealloc(table, i + 1);
		table->registry[i] = buffer;
		buffer = fgetc(file);

		if(actualLen > table->bigOfCol[actualCol])
			table->bigOfCol[actualCol] = actualLen;


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

	fclose(file);
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

#endif
