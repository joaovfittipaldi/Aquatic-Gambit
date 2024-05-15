#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define AR 0
#define DOCE 1
#define SALGADA 2

typedef struct obj{
  char img;
  int x;
  int y;
}obj;

typedef struct balao{
  char img;
  int conteudo;
  int numero;
  struct balao *prox;
}balao;

typedef struct item{
  char conteudo[30];
}item;

typedef struct player{
  char img;
  int vidas;
}player;

void print_obj(obj *obj);
void print_menu();
void print_controles();
void inserir_pilha(balao **head, char img, int conteudo, int numero);
void remover_pilha(balao **head);
void print_primeiro_pilha(balao *head);
void printar_lista(item *head);
void inserir_balao(balao **head);
void print_balao_total(balao *head);
void print_opcoes(item a[]);
void arremessar_oponente(player **jogador, balao **baloes, item *itensp1, item *itensp2);
void arremessar_si(player **jogador, balao **baloes, item *itensp1, item *itensp2);
void agulha();
void algema();
void recuperar_vida();
void lupa();
void ordenar(item **head);
void jogada(player **jogador, player **jogador2, balao **baloes, item *itensp1, item *itensp2);
void jogo();

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
            if(select.y < 12) //S
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
            print_controles();
          }
          else if(select.y == 12)
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

void inserir_pilha(balao **head, char img, int conteudo, int numero){
  balao *novo = (balao *)malloc(sizeof(balao));

  if(novo != NULL){
      novo->img = img;
      novo->conteudo = conteudo;
      novo->numero = numero;
      novo->prox = *head;
      *head = novo;
  }
}

void remover_pilha(balao **head){
  balao *aux;

  if(*head != NULL){
      aux = *head;
      *head = (*head)->prox;
      free(aux);
  }
}

void print_primeiro_pilha(balao *head){
  if(head != NULL){
      printf("%c -> ", head->img);
  }
}

void print_obj(struct obj *obj)
{
  screenGotoxy(obj->x, obj->y);
  printf("%c", obj->img);
}

void print_menu(){
  screenGotoxy(36, 4);
  printf("AQUATIC GAMBIT");

  screenGotoxy(36, 8);
  printf("START GAME");

  screenGotoxy(36, 10);
  printf("CONTROLS");

  screenGotoxy(36, 12);
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
      printf("AQUATIC GAMBIT");

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

void inserir_balao(balao **head){
  srand(time(NULL));
  
  int posicao_2 = rand() % 8;

  for(int i=8; i>0; i--){
    if(i == posicao_2){
      inserir_pilha(head, 'B', 2, i);
    }
    else{
      int numero;
      do{
        numero = rand() % 3;
      }while(numero == 2);
      inserir_pilha(head, 'B', numero, i);
    }
  }

}

void print_balao_total(balao *head) {
  int ar_count = 0;
  int doce_count = 0;
  int salgada_count = 0;

  balao *current = head;
  while (current != NULL) {
    switch (current->conteudo) {
      case AR:
        ar_count++;
        break;
      case DOCE:
        doce_count++;
        break;
      case SALGADA:
        salgada_count++;
        break;
    }
    current = current->prox;
  }

  screenGotoxy(30, 15);
  printf("Total de baloes: AR(%d) DOCE(%d) SALGADA(%d)", ar_count, doce_count, salgada_count);
}

void print_opcoes(item a[]){
  screenGotoxy(30, 12);
  printf("Agulha x%d", a[0].conteudo);

  screenGotoxy(30, 14);
  printf("Algema x%d", a[1].conteudo);

  screenGotoxy(30, 16);
  printf("Recuperar vida x%d", a[2].conteudo);

  screenGotoxy(30, 18);
  printf("Lupa x%d", a[3].conteudo);

  screenGotoxy(30, 20);
  printf("Ordenar x%d", a[4].conteudo);

  screenGotoxy(30, 22);
  printf("Shoot the opponent");

  screenGotoxy(30, 24);
  printf("Explode in yourself");
}

void agulha(balao **baloes){
  printf("%s", (*baloes)->conteudo);
  remover_pilha(baloes);
}

void algema(){
  printf("Algema");
}

void recuperar_vida(player **jogador){
  (*jogador)->vidas++;
  printf("Vida recuperada");
}

void lupa(balao **baloes){
  printf("%s", (*baloes)->conteudo);
}

void ordenar(item **head){
  printf("Ordenar");
}

void jogada(player **jogador1, player **jogador2, balao **baloes, item *itensp1, item *itensp2) {
  static int caracter = 0;

  struct obj select;
  select.img = '>';
  select.x = 30;
  select.y = 15;

  print_opcoes(itensp1);
  print_obj(&select);

  while(1){
    print_opcoes(itensp1);
    int enter_press = 0;
    int mov_menu = 1;

    if(keyhit())
    {
      caracter = readch();
    }
    switch(caracter)
    {
      case 119: //W
      if(select.y > 12)
      {
        select.y -= 2;
      }
      break;

      case 115:
        if(select.y < 24) //S
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
      print_opcoes(itensp1);
      print_obj(&select);
    }

    if(enter_press)
    {
      if(select.y == 12)
      {
        agulha(&baloes);
      }
      else if(select.y == 14)
      {
        algema();
      }
      else if(select.y == 16)
      {
        recuperar_vida(&jogador);
      }
      else if(select.y == 18)
      {
        lupa(&baloes);
      }
      else if(select.y == 20)
      {
        ordenar(&baloes);
      }
      else if(select.y == 22)
      {
        arremessar_oponente(&jogador1, &baloes, itensp1, itensp2);
      }
      else if(select.y == 24)
      {
        arremessar_si(&jogador2, &baloes, itensp1, itensp2);
      }
    }
    caracter = 0;
  }
}


void arremessar_oponente(player **jogador, balao **baloes, item *itensp1, item *itensp2){
  if((*baloes)->conteudo == 2){
    (*jogador)->vidas--;
  }
  else if((*baloes)->conteudo == 1){
    printf("Continua\n");
  }
  else{
    printf("Perde a vez\n");
  }

  remover_pilha(baloes);
}

void arremessar_si(player **jogador, balao **baloes, item *itensp1, item *itensp2){
  if((*baloes)->conteudo == 2){
    (*jogador)->vidas--;
  }
  else if((*baloes)->conteudo == 1){
    printf("Perde a vez\n");
  }
  else{
    printf("Continua\n");
  }
}

void jogo(){
  screenInit(1);
  keyboardInit();
  srand(time(NULL)); //inicializa o gerador de números aleatórios

  static int ch = 0;

  player p1, p2; //cria os jogadores
  p1.vidas = 4;
  p2.vidas = 4;

  item array[5]; //cria o array de itens
  strcpy(array[0].conteudo, "Agulha");
  strcpy(array[1].conteudo, "Algema");
  strcpy(array[2].conteudo, "Lupa");
  strcpy(array[3].conteudo, "Ordenar");
  strcpy(array[4].conteudo, "Recuperar vida");

  item itensp1[2];
  item itensp2[2];

  int i, j;
  for (i = 0; i < 2; i++) {
    int random_index = rand() % 5; 
    strcpy(itensp1[i].conteudo, array[random_index].conteudo); 
  }

  for (j = 0; j < 2; j++) {
    int random_index = rand() % 5; 
    strcpy(itensp2[j].conteudo, array[random_index].conteudo); 
  }


  balao *head = NULL; 
 inserir_balao(&head); //cria a pilha de balões
 print_balao_total(head); //printa o total de balões

 while(p1.vidas > 0 && p2.vidas > 0){ //enquanto os jogadores tiverem vidas
    jogada(&p1, &p2, head, itensp1, itensp2); //jogada do jogador 1
    jogada(&p2, &p1, head, itensp1, itensp2); //jogada do jogador 2

 } 


  //printa a tela de fim de jogo, com pontuação obtida e opção de reiniciar ou sair
  screenSetColor(GREEN, BLACK);
  screenGotoxy(35, 4);
  printf("GAME OVER\n");

  screenGotoxy(30, 8);
  if(p1.vidas > 0)
    printf("WINNER: PLAYER 1");
  else
    printf("WINNER: PLAYER 2");

  screenGotoxy(30, 18);
  printf("PRESS 'ENTER' TO EXIT");

  screenUpdate();

  int verif = 1;
  while(verif) 
    {
      if(keyhit())
      {
        ch = readch();
      }

      switch(ch)
        {
          case 10:  //se apertar enter, sai do loop, consequentemente do jogo, e volta para o menu
          verif = 0;
          break;
        }
    }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

}
