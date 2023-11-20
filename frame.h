#ifndef FRAMES_H
#define FRAMES_H

#include "image.h"
#include "table.h"
#include "multOS.h"

typedef struct{
	char *title;
	char *mode;

	Image image;

	char *inputBuffer;
	int inputLen;

	int cursor[2];
	int optionSelect;
	char *options;
	int qtdOptions;

	Table *table;

}Frame;

void frameSetup(Frame *frame, char *title, int width, int height, char _realloc);
void repairFrame(Frame *frame);

void frameSetCursor(Frame *frame, int x, int y);

void frameClean(Frame *frame);
void frameCopy(Frame *frameDestiny, Frame *frameOrigin,int x, int y);
void frameResize(Frame *frame, int newWidth, int newHeight);

void frameInsert(Frame *frame,char *string, int x, int y);
void frameInsertCenter(Frame *frame,char *string, int y);
void frameInsertOption(Frame *frame, char *optionName);
void frameCleanOption(Frame *frame);
void frameCursorAdd(Frame *frame);
void frameCursorSub(Frame *frame);

void frameSetTable(Frame *frame, Table *table);
void frameTableTitles(Frame *frame);

//IMPLEMENTAR
void frameSetInput(Frame *frame, char *input);
void frameInputAdd(Frame *frame, char input);
void frameInputSub(Frame *frame);
void frameInputClean(Frame *frame);

void frameRefresh(Frame *frame);

void frameFree(Frame *frame);

//-------//

void frameSetup(Frame *frame,char *title ,int width, int height, char _realloc){

	width += 2;
	height += 2;
	int i = 0, cursor = width, jump = width -1;
	int sizeTitle = strLen(title);
	int xTitle = (width/2) - (sizeTitle/2);

	frame->image.width = width;
	frame->image.height = height;

	if(_realloc == 'y'){
		frame->title = (char *)realloc(frame->title, sizeTitle +1);
	}
	else{
		frame->title = (char *)malloc(sizeTitle +1);
		frame->mode = (char *)malloc(15);
		strCopy(frame->mode, "menu");

		frame->options = (char *)malloc(2);
		frame->options[0] = '\0';

		frame->inputBuffer = (char *)malloc(2);
		frame->inputBuffer[1] = '\0';
	}

	frame->optionSelect = 0;

	strCopy(frame->title, title);

	imageSetup(&frame->image, width+1, height+1, _realloc);

	if(frame->mode[0] == 'm' && _realloc == 'n'){

		frame->cursor[0] = 0;
		frame->cursor[1] = 0;

		frameSetCursor(frame, 2, 4);
	}

	for(i ; i < width; i++)
		frame->image.pixels[i] = '=';

	imageInsert(&frame->image, title, xTitle, 0);

	height--;
	for(i = 0; i < height; i++){
		frame->image.pixels[cursor] = '|';
		cursor += jump;
		frame->image.pixels[cursor++] = '|';
	}

	cursor -= jump--;

	for(i = 0; i < jump; i++)
		frame->image.pixels[cursor++] = '-';
}

void frameRepair(Frame *frame){

	int i, cursor = 0;
	int size = frame->image.width * frame->image.height;
	int jump = frame->image.width-1;
	int sizeTitle = strLen(frame->title);
	int xTitle = (frame->image.width/2) - (sizeTitle/2);


	for(i = 0; i < size; i++){
		if(frame->image.pixels[cursor] == '-')
			frame->image.pixels[cursor] = ' ';

		cursor++;
	}

	cursor = 0;
	for(i = 0; i < jump; i++)
		frame->image.pixels[cursor++] = '=';

	imageInsert(&frame->image, frame->title, xTitle, 0);

	for(i = 0; i < frame->image.height -1; i++){
		frame->image.pixels[cursor] = '|';
		cursor += jump-1;
		frame->image.pixels[cursor++] = '|';
	}

	jump--;
	cursor -= jump;

	for(i = 1; i < jump; i++)
		frame->image.pixels[cursor++] = '-';
}

void frameSetCursor(Frame *frame, int x, int y){

	int oldX = frame->cursor[0];
	int oldY = frame->cursor[1];

	if(oldY > 0)
		frame->image.pixels[(oldY*(frame->image.width-1)) + oldX + 1] = ' ';

	frame->cursor[0] = x;
	frame->cursor[1] = y;

	frame->image.pixels[(y*(frame->image.width-1)) + x + 1] = '>';
}

void frameClean(Frame *frame){

	imageClean(&frame->image);
	frameSetup(frame, frame->title, frame->image.width - 3, frame->image.height -3, 'y');
}

void frameCopy(Frame *frameDestiny, Frame *frameOrigin,int x, int y){

	int cursor = 0;
	int size = frameOrigin->image.width * frameOrigin->image.height;

	for(int i = 0; i < frameOrigin->image.width-1; i++)
		frameOrigin->image.pixels[cursor++] = ' ';

	while(cursor < size){
		if(frameOrigin->image.pixels[cursor] == '|' || frameOrigin->image.pixels[cursor] == '-')
			frameOrigin->image.pixels[cursor] = ' ';

		cursor++;
	}

	strCopy(frameDestiny->inputBuffer, frameOrigin->inputBuffer);
	imageCopy(&frameDestiny->image, &frameOrigin->image, 0, 0);
	frameRepair(frameDestiny);
}

void frameResize(Frame *frame, int newWidth, int newHeight){

	Frame frameBuffer;
	frameSetup(&frameBuffer, frame->title, frame->image.width-3, frame->image.height-3,'n');
	frameCopy(&frameBuffer, frame, 0, 0);
	frameRepair(&frameBuffer);
	imageShow(&frameBuffer.image);

	frameSetup(frame, frame->title, newWidth-3, newHeight-3, 'y');
	frameClean(frame);

	frameCopy(frame, &frameBuffer, 0, 0);
	imageShow(&frame->image);
}

void frameInsert(Frame *frame,char *string, int x, int y){

	x--;
	if(frame->mode[0] == 'm' && y > 4 + frame->qtdOptions)
		x = -y + 2;

	if(frame->image.width < x + strLen(string)+4)
		frameResize(frame, x + strLen(string)+1, frame->image.height);

	imageInsert(&frame->image, string, x, y);
}

void frameInsertCenter(Frame *frame,char *string, int y){

	int x, len, cursor = 0;
	len = strLen(string) + 1;

	x = (frame->image.width/2) - (len/2);

	if(frame->image.width < x + len +4)
		frameResize(frame, x + len+1, frame->image.height -3);

	x = ((frame->image.width/2) - (len/2)) - 3;

	cursor = (y*(frame->image.width-1));
	for(int i = 0; i < frame->image.width; i++){
		if(frame->image.pixels[cursor] != '|' && frame->image.pixels[cursor] != ':')
			frame->image.pixels[cursor] = ' ';

		cursor++;
	}

	imageInsert(&frame->image, string, x, y);
}

void frameInsertOption(Frame *frame, char *optionName){

	int lenOrigin = strLen(optionName);
	int lenDestiny = strLen(frame->options);
	int newSize = lenOrigin + lenDestiny +1;
	int qtdOptions = 0;

	frame->options = (char *)realloc(frame->options, newSize);
	strCat(frame->options, optionName);
	strCat(frame->options, "|");

	for(int i = 0; i < newSize; i++)
		if(frame->options[i] == '|')
			qtdOptions++;

	frame->qtdOptions = qtdOptions;
	frameInsert(frame, optionName, 2 - qtdOptions,3 + qtdOptions);
}

void frameCleanOption(Frame *frame){

	int len = strLen(frame->options);

	for(int i = 0; i < len; i++)
		frame->options[i] = '\0';

	frame->qtdOptions= 0;
	frame->options = (char *)realloc(frame->options,2);
	frame->options[0] = '\0';
}

void frameCursorAdd(Frame *frame){

	if(frame->cursor[1] - 3 == frame->qtdOptions){
		frameSetCursor(frame,2, 4);
		frame->optionSelect = 0;
	}
	else{
		frameSetCursor(frame,2,frame->cursor[1] + 1);
		frame->optionSelect++;
	}
}

void frameCursorSub(Frame *frame){

	if(frame->cursor[1] - 4 == 0){
		frameSetCursor(frame,2, 3 + frame->qtdOptions);
		frame->optionSelect = frame->qtdOptions - 1;
	}
	else{
		frameSetCursor(frame,2,frame->cursor[1] - 1);
		frame->optionSelect--;
	}
}

void frameSetTable(Frame *frame, Table *table){

	frame->mode[0] = 't';
	frame->table = table;
	frameRefresh(frame);
	tableRefresh(frame->table);
}

void frameTableTitles(Frame *frame){

	int i = 0, j = 0;
	char *buffer = (char *)malloc(4);
	int newWidth = 0;
	int newHeight = frame->image.height;
	int cursorTable = 1;
	int cursorBuffer = 0;
	int cursorTitles = 0;
	int bigOfCol;

	for(j = 0; j < frame->table->qtdCol; j++){
		newWidth += frame->table->bigOfCol[j] + 2;
	}

	buffer = (char *)realloc(buffer, newWidth);

	if(newWidth * newHeight > frame->image.width * frame->image.height)
		frameResize(frame, newWidth, newHeight);

	for(i = 0; i < frame->table->qtdCol; i++){
		bigOfCol = frame->table->bigOfCol[i];

		cursorBuffer = 0;

		while(frame->table->colTitles[cursorTitles] != ',' && frame->table->colTitles[cursorTitles] != '\0'){
			buffer[cursorBuffer] = frame->table->colTitles[cursorTitles];
			cursorBuffer++;
			cursorTitles++;
		}

		buffer[cursorBuffer] = '\0';

		frameInsert(frame, buffer, cursorTable, 1);
		cursorTable += bigOfCol + 2;
		frameInsert(frame, ":", cursorTable++, 1);

		cursorTitles++;
	}

	for(i = 0; i < frame->image.width-4; i++)
		frameInsert(frame, "-", i, 2);

	free(buffer);
}

void frameTableNext(Frame *frame){

	if(frame->table->actualPage < frame->table->qtdLine)
		frame->table->actualPage++;
}

//IMPLEMENTAR
//void frameSetInput(Frame *frame, char *input);
//void frameInputAdd(Frame *frame, char input);
//void frameInputSub(Frame *frame);
//void frameInputClean(Frame *frame);

void frameRefresh(Frame *frame){

	int i = 0, j = 0;

	if(frame->mode[0] == 't'){
		int jumpPage = 0;
		int pages = false, nextAvaliable = false, prevAvaliable = false;
		char *buffer = (char *)malloc(frame->image.width);
		int cursor = 1;

		frameClean(frame);
		frameTableTitles(frame);

		if(frame->image.height - 6 < frame->table->qtdLine)
			pages = true;

		//listRefresh
		if(frame->table->actualPage - frame->table->qtdLine + frame->image.height < 5)
			nextAvaliable = true;
		if(frame->table->actualPage > 0)
			prevAvaliable = true;

		if(frame->inputBuffer[0] == 's' || frame->inputBuffer[0] == 'S'){
			if(nextAvaliable){
				frame->table->actualPage++;

				if(frame->table->actualPage +1 - frame->table->qtdLine + frame->image.height > 5)
					nextAvaliable = false;
				prevAvaliable = true;
				tableRefresh(frame->table);
			}
		}
		else if(frame->inputBuffer[0] == 'w' || frame->inputBuffer[0] == 'W'){
			if(prevAvaliable){
				frame->table->actualPage--;

				if(!frame->table->actualPage)
					prevAvaliable = false;
				nextAvaliable = true;
				tableRefresh(frame->table);
			}
		}

		jumpPage = frame->table->actualPage;

		//Frame Draw
		for(i = 0; i < frame->image.height - 6; i++){
			cursor = 1;

			if(i + jumpPage < frame->table->qtdLine){
				for(j = 0; j < frame->table->qtdCol; j++ ){
					tableGet(frame->table, j, i + jumpPage, buffer);
					frameInsert(frame, buffer, cursor - i + 1, i + 3);

					cursor+= frame->table->bigOfCol[j] + 2;
				}
			}
		}

		//Footer
		if(pages && prevAvaliable && nextAvaliable)
			frameInsertCenter(frame,"W-Anterior : S-Proximo",frame->image.height - 2);
		else if(pages && nextAvaliable && !prevAvaliable)
			frameInsertCenter(frame,"S-Proximo",frame->image.height - 2);
		else if(pages)
			frameInsertCenter(frame,"S-Anterior",frame->image.height - 2);

		free(buffer);
	}
}

void frameFree(Frame *frame){

	free(frame->title);
	free(frame->mode);
	imageFree(&frame->image);
	free(frame->inputBuffer);
	free(frame->options);
}

#endif
