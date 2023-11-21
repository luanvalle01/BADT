/******************************************************************************/
/* !CÓDIGO GETCH RETIRADO DE:                                                 */
/* https://bitismyth.wordpress.com/2015/06/10/um-getch-multiplataforma/       */
/* (acesso em 08 de novembro de 2023 às 23:21)                                */	
/******************************************************************************/

/******************************************************************************/
/* ARQUIVO DEDICADO A FUNÇÕES COMPATÍVEIS COM AMBOS SO's                      */
/* (Necessário para BADT V 1.0)                                               */
/* https://github.com/luanvalle01/BADT                                        */
/*                                                                            */
/******************************************************************************/

#ifndef MULTOS_H
#define MULTOS_H

#ifdef __linux
  #include <unistd.h>
  #include <termios.h>
#else
  #include <windows.h>
#endif
 
int getch(void)
{
#ifdef __linux
  struct termios newtios, oldtios;
  int ch;
 
  tcgetattr(STDIN_FILENO, &oldtios);
 
  /* Reusa o modo atual. */
  newtios = oldtios;
 
  /* Desabilita o modo canônico e o echo */
  newtios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newtios);
 
  ch = getchar();
 
  /* Retorna o terminal ao normal. */
  tcsetattr(STDIN_FILENO, TCSANOW, &oldtios);
 
  return ch;
#else
  // Usa as funções do console do Windows para ler uma tecla.
  //
  HANDLE hStdin = GetStdHandle (STD_INPUT_HANDLE);
  INPUT_RECORD irInputRecord;
  DWORD dwEventsRead;
  int ch;
 
  /* Lê keypress... */
  while (ReadConsoleInputA(hStdin, &irInputRecord, 1, &dwEventsRead))
    if (irInputRecord.EventType == KEY_EVENT &&
    irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_SHIFT &&
    irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_MENU &&
    irInputRecord.Event.KeyEvent.wVirtualKeyCode != VK_CONTROL)
    {
      ch = irInputRecord.Event.KeyEvent.uChar.AsciiChar;
 
      /* ... e, finalmente, lê o key release! */
      ReadConsoleInputA (hStdin, &irInputRecord , 1, &dwEventsRead);
      return ch;
    }
 
  return EOF;
#endif
}

//-----------------------//

void clear(){

	#ifdef __linux
  		system("clear");
	#else
 		 system("cls");
	#endif
}

#endif
