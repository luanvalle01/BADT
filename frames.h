#include <stdlib.h>
#include <string.h>

typedef struct{
	int width, height;
	char *image;
	char *title;
	char border;
}frame;


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
		if(j >= xTitle &&  j < xTitleFinal){
			Frame.image[j] = Frame.title[cursor];
			cursor++;
		}
		else{
			Frame.image[j] = '=';
		}
	}

	cursor = Frame.width;

	for(i = 1; i < Frame.height; i++){
		for(j = 0; j < Frame.width; j++){
			if(i == Frame.height - 1)
				Frame.image[cursor] = '-';
			else if(j == 0 || j == Frame.width - 1)
				Frame.image[cursor] = Frame.border;
			else
				Frame.image[cursor] = ' ';
			cursor++;
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

frame frameSetup(char *title,char border ,int width, int height){
	frame Frame;
	Frame.width = width;
	Frame.height = height;

	int sizeTitle = strlen(title)+1;


	Frame.image = (char *)malloc(width*height);
	Frame.title = (char *)malloc(sizeTitle);

	strcpy(Frame.title,title);
	Frame.border = border;

	return Frame;
}

frame frameResize(frame Frame,int newWidth,int newHeight){
	int cursor = Frame.width,cursorBuffer = newWidth,i = 1,j = 1;

	frame frameBuffer = frameSetup(Frame.title,Frame.border,newWidth,newHeight);
	constructorWindow(frameBuffer);

	for(i = 1; i < frameBuffer.height - 1; i++){
		for(j = 0; j < frameBuffer.width; j++){

			if(frameBuffer.image[cursorBuffer] == frameBuffer.border){
				cursorBuffer++;
			}

			if(Frame.image[cursor] == Frame.border){
				cursor++;
			}

			if(cursor > (Frame.width * Frame.height)-Frame.width-1 || cursorBuffer > newWidth * newHeight)
				break;

			if(Frame.image[cursor] != Frame.border && frameBuffer.image[cursorBuffer] != frameBuffer.border)
				frameBuffer.image[cursorBuffer++] = Frame.image[cursor++];

			//Pula a linha do Buffer caso não exista mais dados na origem
			if(j == Frame.width-2){
				while(j < frameBuffer.width-1){
					j++;
					cursorBuffer++;
				}
			}

			//Pula a linha da origem caso não exista mais espaço no buffer
			if(j == newWidth -2 && newWidth < Frame.width){
				int count = 0;

				while(count < Frame.width - newWidth){
					count++;
					cursor++;
				}
			}
		}
	}
	free(Frame.image);
	free(Frame.title);

	return frameBuffer;
}

frame frames(int frameID){
	frame Frame = frameSetup("NONE",'|',1,1);

	switch(frameID){
		case 0:
			Frame = frameSetup("POP UP",'!',10,3);
			constructorWindow(Frame);

			Frame = frameResize(Frame,20,4);

			return  Frame;
			break;
		case 1:
			Frame = frameSetup("JANELA",'|',20,5);
			constructorWindow(Frame);

			constructorLine(Frame,1,"|infossssssssssssss|");
			constructorLine(Frame,2,"|                  |");
			constructorLine(Frame,3,"|>testesaaaaaaaaaaa|");
			constructorLine(Frame,4,"|__________________|");

			Frame = frameResize(Frame,40,10);
			Frame = frameResize(Frame,50,Frame.height+10);
			return Frame;
			break;
	}
}
