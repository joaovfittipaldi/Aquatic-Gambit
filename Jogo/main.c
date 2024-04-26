#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

typedef struct obj
{
  char img;
  int x;
  int y;
} obj;

void print_obj(obj *obj);
void print_menu();
void print_controles();

int main() 
{
  screenInit(1);
  keyboardInit();
  timerInit(100);

  static int caracter = 0;

  obj select;
  select.img = '>';
  select.x = 34;
  select.y = 8;

  print_menu();
  print_obj(&select);

  while(1)
    {
      print_menu();
      int enter_press = 0;
      int mov_menu = 1;

      if(keyhit())
      {
        caracter = readch();
      }
      switch(caracter)  //jogador se movimenta para cima ou baixo e aperta enter para confirmar
        {
          case 119: //W
          if(select.y > 8)
          {
            select.y -= 2;
          }
          break;

          case 115:
            if(select.y < 14) //S
            {
              select.y += 2;
            }
          break;

          case 32:
          enter_press = 1;
          break;

          default:
          mov_menu = 0;
          break;
        }

        if(mov_menu)
        {
          screenClear();
          print_menu();
          print_obj(&select);
        }

        if(enter_press)   //verifica a opção selecionada
        {
          if(select.y == 8)
          {
            screenClear();
            jogo();
          }
          else if(select.y == 10)
          {
            screenClear();
            jogo_2();
          }
          else if(select.y == 12)
          {
            screenClear();
            print_controles();
          }
          else if(select.y == 14)
          {
            screenClear();
            break;
          }
        }
      caracter = 0;

    }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}

void print_obj(struct obj *obj)
{
  screenGotoxy(obj->x, obj->y);
  printf("%c", obj->img);
}

void print_menu(){
  screenGotoxy(36, 4);
  printf("nome jogo");

  screenGotoxy(36, 8);
  printf("1 PLAYER");

  screenGotoxy(36, 10);
  printf("2 PLAYERS");

  screenGotoxy(36, 12);
  printf("CONTROLS");

  screenGotoxy(36, 14);
  printf("EXIT");
}


void print_controles()
{
  struct obj select;
  select.img = '>';
  select.x = 28;
  select.y = 20;

  while(1) //loop para repetir até o jogador apertar enter
    {
      screenGotoxy(36, 4);
      printf("MAHARAJA");

      screenGotoxy(30, 6);
      printf("X -> PLAYER");

      screenGotoxy(30, 7);
      printf("O -> GOAL");

      screenGotoxy(30, 9);
      printf("MOVEMENTS: WASD");

      screenGotoxy(30, 11);
      printf("PRESS SPACE ON TOP OF GOAL ");

      screenGotoxy(30, 13);
      printf("THE MORE YOU WALK, MORE POINTS YOU GET");

      screenGotoxy(30, 15);
      printf("BE CAREFUL WITH THE TIME");

      screenGotoxy(30, 17);
      printf("GAME ENDS AFTER 10 GOALS");

      screenGotoxy(30, 20);
      printf("BACK");

      print_obj(&select);

      static int ch = 0;

      if(keyhit()) //detecta se alguma tecla foi pressionada
      {
        ch = readch(); //lê a tecla
      }
      if(ch == 32)
      {
        screenClear();
        break; //se for enter, sai do loop, consequentemente da função
      }
    }
}
