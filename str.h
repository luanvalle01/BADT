int strLen(char *string){

	int len = 0;
	while(string[len++] != '\0')
		continue;

	return --len;
}

void strCopy(char *strDestiny, char *strOrigin){
	
	int size = strLen(strOrigin);
	for(int i = 0; i < size; i++)
		strDestiny[i] = strOrigin[i];
}

void getBiggest(char *string, int *bigLen){

	int len = 0;
	while(string[len++] != '\0')
		continue;

	if(len > *bigLen)
		*bigLen = len;
}
