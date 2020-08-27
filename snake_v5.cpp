/**
 * NomPrograma: snake_final.cpp
 * Descripcion: El programa es el clásico juego de la serpiente
 * con algunos añadidos como una skin mejorada, un menu, audios,
 * y código poco eficiente :'
 * Nota: Es necesario descargar e instalar allegro 4.2 para compilar el código.
 * Autor: SALVADOR SOEL, Edson Daniel
 * Fecha: 17/07/2019
 * Mejorar: -Bug que finaliza el programa cuando termina la musica de fondo.
 * -Ordenar el menú.
 * -Ordenar todo.
 */
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <lib/allegro.h>
#include <ctime>
#include <sstream>
#include <wchar.h>
#include <locale.h>

#define LARGO 1000
#define ANCHO 720
#define LIMITE_NODOS 200
#define ARRIBA 0
#define DERECHA 1
#define ABAJO 2
#define IZQUIERDA 3
using namespace std;
struct score
{
   int puntaje;
   char nombre[11];
};
struct nodo
{
   int x, y, z;
};
nodo cuerpo[LIMITE_NODOS];
void setup();
void inicio(bool *op);
void dibujar_bordes();
void actualizar_pantalla();
void generar_snake();
void dibujar_snake();
void actualiza_direccion();
void mover_snake();
void generar_comida();
void dibujar_comida();
void dibujar_puntos();
void dibujar_vidas();
void iniciar_score();
void dibujar_scoremax();
void almacenar_score();
void menu();
void ttop_5();

BITMAP *buffer;
BITMAP *cabezas;
BITMAP *cabeza;
BITMAP *cuerpos;
BITMAP *cuerpo1;
BITMAP *curvas;
BITMAP *colas;
BITMAP *cola;
BITMAP *corazon;
BITMAP *comida;
BITMAP *menu1;
BITMAP *jugar;
BITMAP *salir;
BITMAP *top5;
BITMAP *jugar_off;
BITMAP *salir_off;
BITMAP *top5_off;
BITMAP *fondo;
BITMAP *ayuda;

FILE *F;
int LIMITE_X=LARGO/40;
int LIMITE_Y=ANCHO/40;
int TAM=3;
int DIR=DERECHA;
int COMIDA_X, COMIDA_Y;
int PUNTOS=0;
int VIDAS=3;
int VELOCIDAD=100;
int PUNTAJE_MAX=0;
int OPCION=3;
int i=0;

int main()
{
   setup();
   if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0){
      allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
      return 1;
   }
   set_volume(230, 200);
   bool o=false;
   SAMPLE *MUSICA1  = load_wav("midi/wolves.wav");
   //MIDI *MUSICA2  = load_midi("seal.mid");
   SAMPLE *SONIDO1 = load_wav("midi/papel_megasu.wav");

   while(!key[KEY_X]){
//      if(i==0){
//         play_sample(MUSICA1, 200, 150, 1000,0);
//      }
//      i++;
      clear(buffer);
      menu();
      actualizar_pantalla();
      rest(50);
      if(key[KEY_ENTER]){
         o=true;
         play_sample(SONIDO1,200,150,1000,0);
      }
      if(o){
         switch(OPCION){
            case 1:
               exit(3);
               break;
            case 2:
               while(!key[KEY_P]){
                  ttop_5();
                  actualizar_pantalla();
                  rest(50);
                  o=false;
               }
               break;
            case 3:
               //play_midi(MUSICA2, 1);
               inicio(&o);
               dibujar_bordes();
               generar_snake();
               generar_comida();
               dibujar_comida();
               dibujar_puntos();
               dibujar_vidas();
               iniciar_score();
               dibujar_scoremax();
               actualizar_pantalla();
               while(!key[KEY_ESC]){
                  clear(buffer);
                  dibujar_bordes();
                  actualiza_direccion();
                  mover_snake();
                  dibujar_vidas();
                  dibujar_scoremax();
                  dibujar_snake();
                  dibujar_puntos();
                  dibujar_comida();
                  if(VIDAS==0){
                     break;
                  }
                  actualizar_pantalla();
                  rest(VELOCIDAD);
               }
               almacenar_score();
               break;
         }
      }
   }
   destroy_sample(MUSICA1);
   destroy_sample(SONIDO1);
   destroy_bitmap(buffer);
   return(0);
}
END_OF_MAIN()

void setup()
{
   allegro_init();
   install_keyboard();
   set_color_depth(32);
   set_gfx_mode(GFX_AUTODETECT_WINDOWED, LARGO, ANCHO, 0, 0);
   srand(time(NULL));

   buffer=create_bitmap(LARGO, ANCHO);
   cabezas=create_bitmap(160,40);
   cabeza=create_bitmap(40,40);
   cuerpos=create_bitmap(160,40);
   cuerpo1=create_bitmap(40,40);
   curvas=create_bitmap(160,40);
   colas=create_bitmap(160,40);
   cola=create_bitmap(40,40);
   corazon=create_bitmap(40,40);
   comida=create_bitmap(40,40);
   menu1=create_bitmap(LARGO, ANCHO);
   jugar=create_bitmap(320,100);
   salir=create_bitmap(320,100);
   top5=create_bitmap(320,100);
   jugar_off=create_bitmap(320,100);
   salir_off=create_bitmap(320,100);
   top5_off=create_bitmap(320,100);
   fondo=create_bitmap(1000, 680);
   ayuda=create_bitmap(LARGO, ANCHO);

   cabezas=load_bitmap("sprites/cabezas.bmp",NULL);
   cuerpos=load_bitmap("sprites/cuerpos.bmp",NULL);
   curvas=load_bitmap("sprites/curvas.bmp",NULL);
   colas=load_bitmap("sprites/colas.bmp",NULL);
   corazon=load_bitmap("sprites/vida.bmp",NULL);
   comida=load_bitmap("sprites/comida.bmp",NULL);
   menu1=load_bitmap("menu/menu.bmp",NULL);
   jugar=load_bitmap("menu/jugar.bmp",NULL);
   salir=load_bitmap("menu/salir.bmp",NULL);
   top5=load_bitmap("menu/top5.bmp",NULL);
   jugar_off=load_bitmap("menu/jugar_off.bmp",NULL);
   salir_off=load_bitmap("menu/salir_off.bmp",NULL);
   top5_off=load_bitmap("menu/top5_off.bmp",NULL);
   fondo=load_bitmap("sprites/fondo1.bmp",NULL);
   ayuda=load_bitmap("menu/ayuda.bmp", NULL);
}

void dibujar_bordes()
{
   draw_sprite(buffer, fondo, 0, 40);
}

void actualizar_pantalla()
{
   blit(buffer, screen, 0, 0, 0, 0, LARGO, ANCHO);
}

void generar_snake()
{
   ///Posición de la cabeza
   cuerpo[0].x=10;
   cuerpo[0].y=9;
   ///Generar cuerpo
   for(int i=1;i<=TAM;i++){
      cuerpo[i].x=cuerpo[i-1].x-1;
      cuerpo[i].y=cuerpo[0].y;
   }
}

void dibujar_snake()
{
   ///Dibujar cabeza
   switch(DIR)
   {
   case ARRIBA:
      blit(cabezas, cabeza, 0, 0, 0, 0, 40, 40);
      break;
   case DERECHA:
      blit(cabezas, cabeza, 40, 0, 0, 0, 40, 40);
      break;
   case ABAJO:
      blit(cabezas, cabeza, 80, 0, 0, 0, 40, 40);
      break;
   case IZQUIERDA:
      blit(cabezas, cabeza, 120, 0, 0, 0, 40, 40);
      break;
   }
   draw_sprite(buffer,cabeza, cuerpo[0].x*40,cuerpo[0].y*40);
   ///Dibujar cuerpo
   for(int i=1;i<TAM;i++){
      if(cuerpo[i-1].x<cuerpo[i].x && cuerpo[i+1].x>cuerpo[i].x){
         blit(cuerpos, cuerpo1, 40, 0, 0, 0, 40, 40);
      }
      else if(cuerpo[i-1].x>cuerpo[i].x && cuerpo[i+1].x<cuerpo[i].x){
         blit(cuerpos, cuerpo1, 120, 0, 0, 0, 40, 40);
      }
      else if(cuerpo[i-1].y<cuerpo[i].y && cuerpo[i+1].y>cuerpo[i].y){
         blit(cuerpos, cuerpo1, 80, 0, 0, 0, 40, 40);
      }
      else if(cuerpo[i-1].y>cuerpo[i].y && cuerpo[i+1].y<cuerpo[i].y){
         blit(cuerpos, cuerpo1, 0, 0, 0, 0, 40, 40);
      }
      ///Dibujar curvas del cuerpo
      else if(cuerpo[i-1].x<cuerpo[i].x && cuerpo[i+1].y<cuerpo[i].y || cuerpo[i+1].x<cuerpo[i].x && cuerpo[i-1].y<cuerpo[i].y){
         blit(curvas, cuerpo1, 80, 0, 0, 0, 40, 40);
      }
      else if(cuerpo[i-1].x<cuerpo[i].x && cuerpo[i+1].y>cuerpo[i].y || cuerpo[i+1].x<cuerpo[i].x && cuerpo[i-1].y>cuerpo[i].y){
         blit(curvas, cuerpo1, 40, 0, 0, 0, 40, 40);
      }
      else if(cuerpo[i-1].x>cuerpo[i].x && cuerpo[i+1].y<cuerpo[i].y || cuerpo[i+1].x>cuerpo[i].x && cuerpo[i-1].y<cuerpo[i].y){
         blit(curvas, cuerpo1, 120, 0, 0, 0, 40, 40);
      }
      else if(cuerpo[i-1].x>cuerpo[i].x && cuerpo[i+1].y>cuerpo[i].y || cuerpo[i+1].x>cuerpo[i].x && cuerpo[i-1].y>cuerpo[i].y){
         blit(curvas, cuerpo1, 0, 0, 0, 0, 40, 40);
      }
      draw_sprite(buffer, cuerpo1, cuerpo[i].x*40,cuerpo[i].y*40);
   }
   ///Dibujar cola
   if(cuerpo[TAM].x<cuerpo[TAM-1].x){
      blit(colas, cola, 40, 0, 0, 0, 40, 40);
   }
   else if(cuerpo[TAM].x>cuerpo[TAM-1].x){
      blit(colas, cola, 120, 0, 0, 0, 40, 40);
   }
   else if(cuerpo[TAM].y>cuerpo[TAM-1].y){
      blit(colas, cola, 0, 0, 0, 0, 40, 40);
   }
   else{
      blit(colas, cola, 80, 0, 0, 0, 40, 40);
   }
   draw_sprite(buffer,cola, cuerpo[TAM].x*40,cuerpo[TAM].y*40);
}

void actualiza_direccion()
{
   if(key[KEY_UP]&&DIR!=ABAJO){
      DIR=ARRIBA;
   }
   else if(key[KEY_DOWN]&&DIR!=ARRIBA){
      DIR=ABAJO;
   }
   else if(key[KEY_RIGHT]&&DIR!=IZQUIERDA){
      DIR=DERECHA;
   }
   else if(key[KEY_LEFT]&&DIR!=DERECHA){
      DIR=IZQUIERDA;
   }
}

void perder_vida()
{
   VIDAS--;
   if(VIDAS>0){
      //clear(screen);
      textout_ex(screen, font,"Has perdido una vida", 40*10,40*9, 0xFFFFFF, 0X000000);
      rest(2000);
   }
   else{
      clear(screen);
      textout_ex(screen, font,"GAME OVER", 450,40*9, 0xFF0000, 0X000000);
      rest(2500);
      almacenar_score();
   }
}

void mover_snake()
{

   ///Rutina para comer
   if(cuerpo[0].x==COMIDA_X&&cuerpo[0].y==COMIDA_Y){
      generar_comida();
      TAM++;
      cuerpo[TAM]=cuerpo[TAM-1];
      PUNTOS=PUNTOS+10;
      if(PUNTOS%100==0 && PUNTOS>0 && VELOCIDAD>30){
         VELOCIDAD=VELOCIDAD-10;
      }
   }
   ///Actualizar la posicion del cuerpo
   for(int i=TAM;i>0;i--){
      cuerpo[i]=cuerpo[i-1];
   }
   ///Actualizar la posicion de la cabeza
   switch(DIR)
   {
   case ARRIBA:
      cuerpo[0].y--;
      break;
   case ABAJO:
      cuerpo[0].y++;
      break;
   case DERECHA:
      cuerpo[0].x++;
      break;
   case IZQUIERDA:
      cuerpo[0].x--;
      break;
   }
   if(cuerpo[0].x<1 || cuerpo[0].x>(LIMITE_X-2) || cuerpo[0].y<2 || cuerpo[0].y>(LIMITE_Y-2)){
      perder_vida();
      generar_snake();
      DIR=DERECHA;
   }
   ///Verificar colisión con el cuerpo
   for(int i=1;i<=TAM;i++){
      if(cuerpo[0].x==cuerpo[i].x&&cuerpo[0].y==cuerpo[i].y){
         perder_vida();
         generar_snake();
         DIR=DERECHA;
         break;
      }
   }
}

void generar_comida()
{
   COMIDA_X=rand()%(LIMITE_X-2)+1;
   COMIDA_Y=rand()%(LIMITE_Y-3)+2;
   int i=0;
   while(i<=TAM){
      if(COMIDA_X==cuerpo[i].x && COMIDA_Y==cuerpo[i].y){
         COMIDA_X=rand()%(LIMITE_X-2)+1;
         COMIDA_Y=rand()%(LIMITE_Y-3)+2;
         int i=0;
      }
      else{
         i++;
      }
   }
}

void dibujar_comida()
{
   draw_sprite(buffer, comida, COMIDA_X*40, COMIDA_Y*40);
}

void dibujar_puntos()
{
   extern char allegro_id[];
   extern int os_multitasking;
   stringstream ss;
   char puntaje[50];
   ss<<"PUNTOS: "<<PUNTOS;
   string p=ss.str();
   strcpy(puntaje, p.c_str());
   textout_ex(buffer, font, puntaje, 50, 15, 0xFFFFFF, 0X000000);
}

void dibujar_vidas()
{
   textout_ex(buffer, font, "VIDAS: ", LARGO-180, 14, 0xFFFFFF, 0x000000);
   for(int i=0;i<VIDAS;i++){
      draw_sprite(buffer, corazon, i*40+LARGO-120,0);
   }
}

void iniciar_score()
{
   F=fopen("score.dat","r");
   if(F!=NULL){
      fread(&PUNTAJE_MAX, sizeof(PUNTAJE_MAX),1,F);
   }
   fclose(F);
}

void dibujar_scoremax()
{
   stringstream ss;
   char puntaje[25];
   if(PUNTAJE_MAX>PUNTOS){
      ss<<"MAXIMO SCORE: "<<PUNTAJE_MAX;
   }
   else{
      ss<<"MAXIMO SCORE: "<<PUNTOS;
   }
   string p=ss.str();
   strcpy(puntaje, p.c_str());
   textout_ex(buffer, font, puntaje, 350, 15, 0xFFFFFF, 0X000000);
}

void almacenar_score()
{
   if(PUNTOS>PUNTAJE_MAX){
      F=fopen("score.dat","w");
      fwrite(&PUNTOS,sizeof(PUNTOS),1,F);
      fclose(F);
   }
}

void menu()
{
   draw_sprite(buffer, menu1, 0, 0);
   draw_sprite(buffer, jugar_off, 339, 260);
   draw_sprite(buffer, top5_off, 339, 370);
   draw_sprite(buffer, salir_off, 339, 480);
   if(key[KEY_UP]){
      OPCION++;
   }
   else if(key[KEY_DOWN]){
      OPCION--;
   }
   if(OPCION>3){
      OPCION=OPCION%3;
   }
   else if(OPCION<1){
      OPCION=OPCION+3;
   }
   switch(OPCION){
      case 3:
         draw_sprite(buffer, jugar, 339, 260);
         break;
      case 2:
         draw_sprite(buffer, top5, 339, 370);
         break;
      case 1:
         draw_sprite(buffer, salir, 339, 480);
         break;
   }
}

void ttop_5()
{
   draw_sprite(buffer, ayuda, 0, 0);
}

void inicio(bool *op)
{
   TAM=3;
   VIDAS=3;
   DIR=DERECHA;
   PUNTOS=0;
   VELOCIDAD=100;
   OPCION=3;
   *op=false;
}

