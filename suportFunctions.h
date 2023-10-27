#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**********************/
/* FILTROS DE ENTRADA */
/**********************/

void cleanStr(char *string,char ref, int size){
	
	/* Limpa todas as posições da    */
	/* string, substituindo por ref  */
	
	//NECESSITA DO CHAR '|' NO COMEÇO DE CADA LINHA
	
	for(int i = 0;i < size;i++)
		string[i] = ref;
}

int sizeofStr(char *string){
	
	/* Retorna o tamanho total da   */
	/* linha de uma matriz de char  */
	
	//NECESSITA DO CHAR '|' NO COMEÇO DE CADA LINHA
	
	int size = 1;
	while(string[size] != '|')
		size++;
		
	return size;
}

int strVerify(char *input, int *letras, int *pontos){

	/* Retorna quantas letras e     */
	/* pontos existem em uma string */

	int quantidade = strlen(input);

	//Referencias ASCII
	char ascii = '0';
	char ponto = '.';

	for(int i = 0; i < quantidade; i++){
		if(input[i] == ponto){
			*pontos += 1;
		}

		//Verifica se o char é um número entre 0 e 9
		int encontrado = 0;
		for(int j = ascii; j < ascii + 10; j ++){
			if(input[i] == j){
				encontrado = 1;
				break;
			}
		}
		//Caso não seja o contador de erros aumenta
		if(encontrado < 1 && input[i] != ponto){
			*letras += 1;
		}
	}
}

void typeCast(char *input, char *output){

	/* Verifica a possibilidade de  */
	/* cast de um input (string)    */
	/* e armazena em um output      */

	int erros = 0;
	int pontos = 0;

	strVerify(input, &erros, &pontos);

	if(pontos == 1 && erros == 0){
		strcpy(output,"float");
	}
	else if(pontos < 1 && erros == 0){
		strcpy(output,"int");
	}
	else{
		strcpy(output,"other");
	}
}

void decimalClean(float *input,int dontClean){
	/**************************/
	/* Limpa uma quantidade   */
	/* pré determinada de     */
	/* casas decimais		  */
	/**************************/
	/* 		(TRUNCA)		  */

	// ! input deve ser uma referência
	// ! dontClean determina quantos dígitos serão preservados
	// ! dontClean 0 limpa todos os dígitos decimais

	if(dontClean > 0){
		int power = 10;
		dontClean -= 1;

		//Calcula a potência de 10 a ser utilizada
		for(int i=0;i<dontClean;i++){
			power = power * 10;
		}

		//Torna a quantidade desejada em inteiros
		float aux = *input * power;
		//elimina todos os demais dígitos decimais
		int truncade = (int)aux;
		//Torna a quantidade desejada em decimais novamente
		*input = (float)truncade / power;
	}
	else
		*input = (int)*input;
}


/****************/
/*  ORDENAÇÃO	*/
/****************/
void selectionSort(int *input,int size,int mode){
	/**************************/
	/* Reordena um vetor int  */
	/*						  */
	/* mode 0 menor para maior*/
	/* mode 1 maior para menor*/
	/**************************/

	int *output = (int *)malloc(size*4);
	int *available  = (int *)malloc(size*4);
	int selectIndex = 0;

	//Available indica se o index já foi utilizado
	for(int i=0;i < size;i++){
		available[i] = 1;
	}

	if(mode == 0){
		int actualNumber = 999999999;

		for(int i=0;i < size;i++){
			for(int j=0;j < size;j++){
				if(input[j] < actualNumber && available[j] == 1){
					actualNumber = input[j];
					selectIndex = j;
				}
			}

			output[i] = input[selectIndex];
			available[selectIndex] = 0;//bloqueia duplicatas

			actualNumber = 999999999;
		}
	}

	if(mode == 1){
		int actualNumber = -999999999;

		for(int i=0;i < size;i++){
			for(int j=0;j < size;j++){
				if(input[j] > actualNumber && available[j]){
					actualNumber = input[j];
					selectIndex = j;
				}

			}

			output[i] = input[selectIndex];
			available[selectIndex] = 0;

			actualNumber = -999999999;
		}
	}

	//Atualiza os valores de input
	for(int i=0;i < size;i++){
		input[i] = output[i];
	}

	free(output);
	free(available);
}

/***********************/
/* GESTÃO DE ARQUIVOS  */
/***********************/
int *txtToVector(int *List,int *qtd,char *directory){
  /******************************/
	/* Preenche um vetor dinâmico */
	/* a partir de um txt					*/
	/* 														*/
	/* deve ser retornado para um	*/
	/* vetor alocado dinamicamente*/
	/*														*/
	/* o arquivo fonte deve estar */
	/* no seguinte formato:				*/
	/*														*/
	/* 1,3,5,7,10,2,15						*/
	/* número virgula número			*/
	/******************************/
	
  FILE *configs;
  int i = 0, buffer;
  
  //Abre o arquivo configs.txt
  configs = fopen(directory, "r");

  if (configs == NULL)
  {
     printf("Arquivo não encontrado!\n");
  }
  
  //Registra a lista
  while (fscanf(configs, "%d,", &buffer) == 1){
  	if(i > 0)
  		List = (int *)realloc(List,i*2*4);
  	List[i] = buffer;
  	i++;
  }
  
  //Atualiza a quantidade total de registros
  *qtd = i;
  
  //Encerra o uso do arquivo
  fclose(configs);
  
  //Retorna a lista
  return List;
}
