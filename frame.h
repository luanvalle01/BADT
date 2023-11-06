#include "image.h"

typedef struct{
	char *title;
	Image image;

	char *inputBuffer;
	int cursor[2];
	//Table *table;
	//Footer *footer;

}Frame;

void frameSetup(Frame *frame, char *title, int width, int height, char _realloc);
void repairFrame(Frame *frame);

void frameSetCursor(Frame *frame, int x, int y);

void frameClean(Frame *frame);
void frameCopy(Frame *frameDestiny, Frame *frameOrigin,int x, int y);
void frameResize(Frame *frame, int newWidth, int newHeight);

//void frameSetTable(Frame *frame, Table *table);
//void frameSetFooter(Frame *frame, Footer *footer);

void frameInsert(Frame *frame,char *string, int x, int y);
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

	if(_realloc == 'y')
		frame->title = (char *)realloc(frame->title, sizeTitle +1);
	else
		frame->title = (char *)malloc(sizeTitle +1);

	strCopy(frame->title, title);

	imageSetup(&frame->image, width+1, height+1, _realloc);
	frame->cursor[0] = 0;
	frame->cursor[1] = 0;

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

	//frame->table = '\0';
	//frame->footer = '\0';
}

void frameRepair(Frame *frame){

	int i, cursor = 0;
	int size = frame->image.width * frame->image.height;
	int jump = frame->image.width-1;
	int sizeTitle = strLen(frame->title);
	int xTitle = (frame->image.width/2) - (sizeTitle/2);

	for(i = 0; i < jump; i++)
		frame->image.pixels[cursor++] = '=';

	imageInsert(&frame->image, frame->title, xTitle, 0);

	for(i = 0; i < frame->image.height - 1; i++){
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

	frame->cursor[0] = x;
	frame->cursor[1] = y;
}

void frameClean(Frame *frame){
	imageClean(&frame->image);
	frameSetup(frame, frame->title, frame->image.width, frame->image.height, 'y');
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

	imageCopy(&frameDestiny->image, &frameOrigin->image, 0, 0);
	frameRepair(frameDestiny);
}

void frameResize(Frame *frame, int newWidth, int newHeight){

	Frame frameBuffer;
	frameSetup(&frameBuffer, frame->title, frame->image.width-2, frame->image.height-4,'n');
	frameCopy(&frameBuffer, frame, 0, 0);

	imageShow(&frameBuffer.image);

	frameSetup(frame, frame->title, newWidth-3, newHeight-4, 'y');
	frameClean(frame);

	frameCopy(frame, &frameBuffer, 0, 0);
	imageShow(&frame->image);
}
