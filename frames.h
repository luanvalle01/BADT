#include <stdlib.h>

typedef struct{
	int width, height;
	char *image;
}frame;


void constructorAll(frame Frame,char *image){
	int pos = 0;
	for(int i = 0; i < Frame.height; i++){
		for(int j = 0; j < Frame.width; j++){
			Frame.image[pos] = image[pos];
			pos++;
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

frame frameSetSize(int width, int height){
	frame Frame;
	Frame.width = width;
	Frame.height = height;

	Frame.image = (char *)malloc(width*height);

	return Frame;
}

frame frames(int frameID){
	frame Frame = frameSetSize(1,1);

	switch(frameID){
		case 0:
			Frame = frameSetSize(5,3);
			constructorAll(Frame,
				"POPUP|   ||___|"
			);

			return  Frame;
			break;
		case 1:
			Frame = frameSetSize(20,5);
			constructorLine(Frame,0,"_______JANELA_______");
			constructorLine(Frame,1,"|infos             |");
			constructorLine(Frame,2,"|                  |");
			constructorLine(Frame,3,"|>testes           |");
			constructorLine(Frame,4,"|__________________|");


			return Frame;
			break;
	}
}
