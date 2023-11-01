int strLen(char *string){

	int len = 0;
	while(string[len++] != '\0')
		continue;

	return len;
}

void getBiggest(char *string, int *bigLen){

	int len = 0;
	while(string[len++] != '\0')
		continue;

	if(len > *bigLen)
		*bigLen = len;
}
