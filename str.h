#ifndef STR_H
#define STR_H

#define true 1
#define false 0

int strLen(char *string){

	int len = 0;
	while(string[len++] != '\0')
		continue;

	return --len;
}

int strLenPlus(char *string, char terminator){

	int len = 0;
	while(string[len++] != terminator)
		continue;

	return --len;
}

void strCopy(char *strDestiny, char *strOrigin){

	int size = strLen(strOrigin);
	for(int i = 0; i < size; i++)
		strDestiny[i] = strOrigin[i];
}

void strCat(char *strDestiny, char *strOrigin){

	int sizeOrigin = strLen(strOrigin);
	int sizeDestiny = strLen(strDestiny);
	int max = sizeOrigin + sizeDestiny;


	for(int i = sizeDestiny; i < max; i++)
		strDestiny[i] = strOrigin[i - sizeDestiny];

	strDestiny[max + 1] = '\0';
}

void toStr(char *output, void *input, char typeInput){

	output = (char *)malloc(30);

	if(typeInput == 'd' || typeInput == 'i'){
		int *buffer = (int *)input;
		sprintf(output,"%d\n", *buffer);
	}

	else if(typeInput == 'f'){
		float *buffer = (float *)input;
		sprintf(output,"%.2f\n", *buffer);
	}

	output = (char *)realloc(output, strLen(output));
}

void getBiggest(char *string, int *bigLen){

	int len = 0;
	while(string[len++] != '\0')
		continue;

	if(len > *bigLen)
		*bigLen = len;
}

#endif
