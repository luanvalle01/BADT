#include <stdio.h>
#include <stdlib.h>
#include "str.h"

typedef struct{
	char *pixels;
	int width, height;
}Image;

void imageSetup(Image *image, int width, int height, char _realoc);

void imageShow(Image *image);

void imageClean(Image *image);
void imageCopy(Image *imageDestiny,Image  *imageOrigin, int x, int y);
void imageResize(Image *image, int newWidth, int newHeight);

void imageInsert(Image *image, char *string, int x, int y);

void freeImage(Image *image);

//-------//

void imageSetup(Image *image, int width, int height, char _realloc){

	int i = 0;
	int size = width * height + 1;

	if(_realloc == 'y')
		image->pixels = (char *)realloc(image->pixels, size);
	else
		image->pixels = (char *)malloc(size);

	for(int i = 0; i < size; i++)
		image->pixels[i] = ' ';

	image->pixels[i] = '\0';
	image->width = width;
	image->height = height;
}

void imageShow(Image *image){

	int size = image->width * image->height;
	int endLine = image->width -1;

	for(int i = 0; i < size; i++){
		if(i % endLine == 0)
			printf("\n");

		printf("%c", image->pixels[i]);
	}
}

void imageClean(Image *image){

	int i = 0;
	int size = image->width * image->height + 1;

	for(int i = 0; i < size; i++)
		image->pixels[i] = ' ';

	image->pixels[i] = '\0';
}

void imageCopy(Image *imageDestiny, Image  *imageOrigin, int x, int y){

	int i = 0, cursorDestiny = 0, cursorOrigin = 0;
	int size = imageOrigin->width * imageOrigin->height;
	int diferenceW;
	char _realloc = 'n';

	//RESIZE
	if(imageOrigin->width + x > imageDestiny->width)
		_realloc = 'y';

	if(imageOrigin->height + y > imageDestiny->height)
		_realloc = 'y';

	if(_realloc == 'y')
		imageSetup(imageDestiny,  imageOrigin->width + x, imageOrigin->height + y, _realloc);

	//CURSOR SEEK
	cursorDestiny = y * imageDestiny->width + x;
	diferenceW = imageDestiny->width - (imageOrigin->width + x);


	//COPY
	for(i = 0; i < size; i++){
		if(cursorOrigin % (imageOrigin->width - 1) == 0 && cursorOrigin != 0)
			for(int j = 0; j < diferenceW; j++)
				cursorDestiny++;
		imageDestiny->pixels[cursorDestiny++] = imageOrigin->pixels[cursorOrigin++];
	}
}

void imageResize(Image *image, int newWidth, int newHeight){

	int newSize = (newWidth * newHeight) +1;

	Image imageBuffer;
	imageSetup(&imageBuffer, image->width, image->height, 'n');
	imageCopy(&imageBuffer, image, 0, 0);

	image->pixels = realloc(image->pixels, newSize);
	imageSetup(image, newWidth, newHeight, 'y');

	imageCopy(image, &imageBuffer, 0, 0);
}


void imageInsert(Image *image, char *string, int x, int y){

	int cursor = (image->width * y) + x;
	int size = strLen(string);

	if(image->width < size + x)
		imageResize(image, size + x, image->height);

	for(int i = 0; i < size; i++)
		image->pixels[cursor++] = string[i];
}

void imageFree(Image *image){
	free(image->pixels);
}
