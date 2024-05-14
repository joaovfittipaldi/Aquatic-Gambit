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
void insertionSort(item **head);
void inserir_balao(balao **head);
void print_balao_total(balao *head);
void print_opcoes(item a[]);
void arremessar_oponente(player **jogador, balao **baloes, item *itensp1, item *itensp2);
void arremessar_si(player **jogador, balao **baloes, item *itensp1, item *itensp2);
void itens(item *itensp1, item *itensp2);
void jogada(player *jogador, balao *baloes, item *itensp1, item *itensp2);
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
  screenGotoxy(30, 15);
  printf("Itens disponíveis: %s %s", a[0].conteudo, a[1].conteudo);

  screenGotoxy(30, 17);
  printf("Shoot the opponent");

  screenGotoxy(30, 19);
  printf("Explode in yourself");

}

void jogada(player *jogador, balao *baloes, item *itensp1, item *itensp2) {
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
      if(select.y > 15)
      {
        select.y -= 2;
      }
      break;

      case 115:
        if(select.y < 19) //S
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
      if(select.y == 15)
      {
        itens(itensp1, itensp2);
      }
      else if(select.y == 17)
      {
        arremessar_oponente(&jogador, &baloes, itensp1, itensp2);
      }
      else if(select.y == 19)
      {
        arremessar_si(&jogador, &baloes, itensp1, itensp2);
      }
    }
    caracter = 0;
  }
}

void itens(item *itensp1, item *itensp2){
  
}

void arremessar_oponente(player **jogador, balao **baloes, item *itensp1, item *itensp2){
  balao *current = baloes;
  int cont = 0;
  while(current != NULL){
    if(current->numero == 1){
      cont++;
    }
    current = current->prox;
  }

  if(cont > 0){
    jogador->vidas--;
  }
  else{
    printf("Você não tem balões de arremesso\n");
  }
}

void arremessar_si(player **jogador, balao **baloes, item *itensp1, item *itensp2){
  balao *current = baloes;
  int cont = 0;
  while(current != NULL){
    if(current->numero == 1){
      cont++;
    }
    current = current->prox;
  }

  if(cont > 0){
    jogador->vidas--;
  }
  else{
    printf("Você não tem balões de arremesso\n");
  }
}

void jogo(){
  srand(time(NULL)); //inicializa o gerador de números aleatórios

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
    jogada(&p1, head, itensp1, itensp2); //jogada do jogador 1
    jogada(&p2, head, itensp1, itensp2); //jogada do jogador 2

 } 



  struct square *head = NULL; //cria o head da lista encadeada
  struct obj persona;
  struct obj goal;

  //inicia o score com 0 e coloca para printar, para deixar a tela do jogo nas cores corretas
  int score = 0; 
  print_score(score); //foi pra ficar bonito

  int cont_casas = 0; //contador de casas andadas
  int cont_goals = 0; //contador de goals já passados

  static int ch = 0;

  //tamanho da matriz
  int max_x = 5; 
  int max_y = 5;

  persona.img = 'x';
  persona.x = getx(max_x/2, max_x); //define a posição do jogador para o meio do tabuleiro
  persona.y = gety(max_y/2, max_y);

  goal.img = 'o';


  screenInit(1);
  keyboardInit();
  timerInit(100);


  //cria o tabuleiro e define as colisões
  criar_matriz(&head, max_x, max_y);
  int lim_dir = getx(max_x, max_x); 
  int lim_esq = getx(0, max_x);
  int lim_cim = gety(0, max_y);
  int lim_bax = gety(max_y, max_y);


  //gera a posição do goal em algum lugar aleatório do tabuleiro
  goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;
  goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;

  sleep(1);


  //printa tudo
  print_matriz(head);
  print_obj(&persona);
  print_obj(&goal);
  print_score(score);

  timerUpdateTimer(4000); //define o timer como 4 segundos

  while(1) //while true enquanto o jogo não acabar
  {
    if(keyhit())
    {
      ch = readch();
    }

    int mov = 1; //verifica se o usuário andou
    int space_press = 0; //verifica se o usuário apertou espaço

    switch(ch) //WASD
      {
        case 119: //w
        if((persona.y - 1) >= lim_cim){  //verifica se o jogador não está na primeira linha do tabuleiro
          persona.y --; //atualiza a posição do jogador
        }
        else{
          mov = 0; //evita que pontue se o jogador bater na parede
        }
        break;

        case 115:
        if((persona.y + 1) < lim_bax){  //verifica se o jogador não está na última linha do tabuleiro
          persona.y ++;
        } 
        else{
          mov = 0; 
        }
        break;

        case 97:
        if((persona.x - 1) >= lim_esq){  //verifica se o jogador não está na primeira coluna do tabuleiro
          persona.x --;
        }
        else{
          mov = 0;
        }
        break;

        case 100:
        if((persona.x + 1) < lim_dir){  //verifica se o jogador não está na última coluna do tabuleiro
          persona.x ++;
        }
        else{
          mov = 0;
        }
        break;

        case 32:  //se o usuário apertar espaço
        space_press = 1;
        break;

        default:  //por padrão o jogador não anda
        mov = 0;
        break;
      }
    if (mov){  //se o jogador andou, aumenta o contador de número de casas e atualiza os prints
      cont_casas ++;
      print_matriz(head);
      print_obj(&persona);
      print_obj(&goal);
    }

    if(persona.x == goal.x && persona.y == goal.y && space_press)  //se o jogador está em cima do goal e apertou espaço
    {
      goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;  //gera uma nova posição do goal
      goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;
      score += cont_casas - 1; //aumenta a pontuação pelo número de casas andadas
      print_score(score);
      cont_casas = 0; //reseta o número de casas andadas
      cont_goals ++; //aumenta o número de goals passados


      //atualiza os prints
      print_matriz(head);
      print_obj(&persona);
      print_obj(&goal);

      timerUpdateTimer(4000); //reseta o timer
    }

    if (timerTimeOver()) //se o tempo acabar, gera uma nova posição e não marca ponto
    {
      goal.x = (rand() % (lim_dir - lim_esq)) + lim_esq;
      goal.y = (rand() % (lim_bax - lim_cim)) + lim_cim;
      cont_casas = 0;
      cont_goals ++;

      print_matriz(head);
      print_obj(&persona);
      print_obj(&goal);

      timerUpdateTimer(4000);
    }

    ch = 0; //reseta a tecla apertada
    screenGotoxy(60, 12);
    float time = (4000 - (float)getTimeDiff())/1000; //printa o tempo que falta para acabar o goal em contagem regressiva e em segundos
    printf("Timer: %.2f", time);

    if (cont_goals == 10) //se 10 goals passarem, sai do loop, encerrando o jogo
    {
      screenClear();

      break;

    }

    screenUpdate(); //atualiza a tela para printar o que não foi printado

  }


  //printa a tela de fim de jogo, com pontuação obtida e opção de reiniciar ou sair
  screenSetColor(GREEN, BLACK);
  screenGotoxy(35, 4);
  printf("GAME OVER\n");

  screenGotoxy(35, 8);
  printf("SCORE:");
  screenGotoxy(43, 8);
  printf("         ");
  screenGotoxy(43, 8);
  printf("%d ", score);

  screenGotoxy(30, 18);
  printf("PRESS 'R' TO RESTART");

  screenGotoxy(30, 20);
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
          case 114: //se apertar r, roda novamente o jogo
          jogo();
          break;

          case 10:  //se apertar enter, sai do loop, consequentemente do jogo, e volta para o menu
          verif = 0;
          break;
        }
    }

  free_list(head); //limpar a memória alocada

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

}
