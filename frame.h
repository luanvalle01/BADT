#include "image.h"

typedef struct{
	char *title;
	image Image;

	char *inputBuffer;
	int cursor[2];
	//Table *table;
	//Footer *footer;

}Frame;

void frameSetup(Frame *frame, int width, int height, char _realloc);

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

	int sizeTitle = strLen(title);
	int xTitle = (width/2) - (sizeTitle/2);

	imageSetup(&Frame->image, width + 2, height+2, _realloc);
	frame->cursor[0] = 0;
	frame->cursor[1] = 0;

	for(int i = 0; i < width; i++)
		frame->image = '=';

	imageInsert(frame->image, title, xTitle, 0);

	//frame->table = '\0';
	//frame->footer = '\0';
}




