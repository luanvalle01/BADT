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

void tableNewLine(Table *table);
int tableCursor(Table *table, int col, int line);
void tableBuffer(Table *table, int initialCursor, char *buffer);
void tableInsert(Table *table, int col, int line, void *value, char type);
void tableInsertS(Table *table, int col, int line, void *value, int size);

int tableGetJump(Table *table, int linesVisible);

void tableToTxt(Table *table,char *directory);
void TxtToTable(Table *table, char *directory);
void tableToVector(Table *table, int col, void *vector);

//-------//
void tableSetup(Table *table, char *types, char *colTitles){

	int lenTypes = strLen(types);

	table->types = (char *)malloc(lenTypes +1);
	strCopy(table->types, types);

	table->qtdLine = 0;

	table->qtdCol = lenTypes/2;
	table->registry = (char *)malloc(2);
	table->registry[0] = '\0';

	table->bigOfCol = (int *)malloc(table->qtdCol * 4);
	table->actualPage = 0;
}

void tableNewLine(Table *table){

	int i = strLen(table->registry);
	int max = i + table->qtdCol * 2;

	table->registry = (char *)realloc(table->registry, max + 4);

	for(i; i < max; i++){
		table->registry[i++] = ' ';
		table->registry[i] = '|';
	}
	table->registry[i + 1] = '\n';
	table->registry[i + 2] = '\0';

	table->qtdLine++;
}

int tableCursor(Table *table, int col, int line){

	int findCursor = false;
	int colCount = 0, lineCount = 0;

	int i = 0;
	while(!findCursor && table->registry[i]){
		if(lineCount = line && colCount == col){
			findCursor = true;
			break;
		}

		if(table->registry[i] == '\n')
			lineCount++;
		else if(table->registry[i] == '|' && lineCount == line)
			colCount++;

		i++;
	}

	return i;
}

void tableBuffer(Table *table, int initialCursor, char *buffer){

	int i = initialCursor;
	buffer = (char *)malloc(strLen(table->registry) + 1);

	while(table->registry[i] != '\0'){
		buffer[i - initialCursor] = table->registry[i];
		table->registry[i++] = ' ';
	}

	buffer[i] = '\0';
}

void tableInsert(Table *table, int col, int line, void *value, char type){

	char *finalValue = (char *)malloc(20);
	char *buffer;
	int lenValue = 0;
	int cursor = 0;
	int i = 0;

	while(line >= table->qtdLine)
		tableNewLine(table);

	if(type == 'i' || type == 'd'){
		int *temp = (int *)value;
		sprintf(finalValue,"%d", temp[0]);
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

	lenValue = strLen(buffer);
	cursor = tableCursor(table, col, line);

	tableBuffer(table, cursor, buffer);

	for(i; i < lenValue; i++)
		table->registry[cursor++] = buffer[i];
	table->registry[cursor++] = '|';

	table->registry = (char *)realloc(table->registry,strLen(table->registry) + 20);

	i = 0;
	while(buffer[i] != '\0')
		table->registry[cursor++] = buffer[i++];

	if(table->bigOfCol[col] < lenValue)
		table->bigOfCol[col] = lenValue;

	free(finalValue);
	free(buffer);
}

void tableInsertS(Table *table, int col, int line, void *value, int size){

	char *finalValue = (char *)malloc(size);
	char *buffer;
	int lenValue = 0;
	int cursor = 0;
	int i = 0;

	while(line >= table->qtdLine)
		tableNewLine(table);

	strCopy(finalValue, value);
	lenValue = strLen(finalValue);

	cursor = tableCursor(table, col, line);

	tableBuffer(table, cursor, buffer);

	for(i; i < lenValue; i++)
		table->registry[cursor++] = buffer[i];
	table->registry[cursor++] = '|';

	table->registry = (char *)realloc(table->registry,strLen(table->registry) + 20);

	i = 0;
	while(buffer[i] != '\0')
		table->registry[cursor++] = buffer[i++];

	if(table->bigOfCol[col] < lenValue)
		table->bigOfCol[col] = lenValue;

	free(finalValue);
	free(buffer);
}

int tableGetJump(Table *table, int initialLine){

	return(tableCursor(table, 0, initialLine));
}

#endif
