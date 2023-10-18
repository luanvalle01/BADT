#include <stdio.h>
#include "frames.h"

void show(frame Frame){
	int pos = 0;
	for(int i=0; i < Frame.height; i++){
		for(int j=0; j < Frame.width; j++){
			printf("%c",Frame.image[pos]);
			pos++;
		}
		printf("\n");
	}
}

void showSuper(frame FrameDown, frame FrameUp){
	int x, y, posDown = 0, posUp = 0;
	int finalX, finalY;

	x = (FrameDown.width/2) - (FrameUp.width/2);
	y = (FrameDown.height/2) - (FrameUp.height/2);

	finalX = (x + FrameUp.width);
	finalY = (y + FrameUp.height)-1;

	for(int i=0; i < FrameDown.height; i++){
		for(int j=0; j < FrameDown.width; j++){
			if(j >=x && j < finalX && i >=y && y < finalY && FrameUp.image[posUp]){
				printf("%c",FrameUp.image[posUp]);
				posUp++;
			}
			else{
				printf("%c",FrameDown.image[posDown]);
			}
			posDown++;
		}
		printf("\n");
	}
}

void preencher(frame Frame){
	int pos = 0;
	for(int i=0; i < Frame.height; i++){
		for(int j=0; j < Frame.width; j++){
			Frame.image[pos] = 'a';
			pos++;
		}
	}
}

void main(){
	frame Frame = frames(0);
	frame Frame2 = frames(1);


	show(Frame);
	show(Frame2);

	printf("\n");

	showSuper(Frame2,Frame);

	free(Frame.image);
	free(Frame2.image);
}
