#include <REG2051.H>
#define UCHAR unsigned char
#define UINT unsigned int

#define READF 0x2B
#define READC 0x54

UCHAR kbHit(void);
void kbInit(void);
UCHAR keyNumber(void);
UCHAR kbRead(void);
void leerclave(UCHAR uc, UCHAR *clv);

void kbInit(void)
   {
   P1 = READF;      /* Listo para leer filas */
   }/* FIN DE kbInit */


UCHAR kbHit(void)
   {
   return (P1 ^ READF); /* Dará 0 si no hay tecla pulsada */
   }/* FIN DE kbHit */

UCHAR keyNumber(void)
   {
   UCHAR fila, col;

   // Leer e identificar fila
   fila = P1;
   switch(fila)
     {
     case 0x0b:    fila=0;    break;   /* 00_0.1011 */
     case 0x2a:    fila=1;    break;   /* 0010.101_ */
     case 0x29:    fila=2;    break;   /* 0010.10_1 */
     case 0x23:    fila=3;    break;   /* 0010._011 */
     default:      fila=0xff; break;
     }

   // Leer e identificar columna
   P1  = READC;  /* Listo para leer columna */
   col = P1;     /* Leer para determinar la columna */
   P1  = READF;  /* Listo para leer columna */
   switch(col)
      {
      case 0x44:  col=0;     break;   /* 010_.0100 */
      case 0x14:  col=1;     break;   /* 0_01.0100 */
      case 0x50:  col=2;     break;   /* 0101.0_00 */
      default:    col=0xff;  break;
      }

   // Identificar la tecla pulsada
   if((fila == 0xff)||(col == 0xff))
      col = 0xff;      			/* Si no hay tecla o hay más de una tecla pulsada */
   else
      col = (col<<2)+fila;    /* Determina el número de la tecla pulsada */

   return(col);
   }/* FIN DE keyNumber */


UCHAR kbRead(void)
   {
   UCHAR uc;

   /* La siguiente tabla se usa para hallar el código de cada tecla */
   /* Tabla de constantes (CODE) */
   UCHAR code kbTable[16] = { '1', '4', '7', '*',   /* Columna 0 */
                              '2', '5', '8', '0',   /* Columna 1 */
                              '3', '6', '9', '#'};  /* Columna 2 */
   int i;

   if (kbHit() == 0) return 0xff;  // Si noy tecla fuera
   while(kbHit() != 0)   
     ;                  /* Esperar a que se suelte la tecla anterior */
 	while(kbHit() == 0)
     ;                  /* Esperar hasta que se pulse una nueva tecla */

   for(i=0; i<3600; i++); /* Pausa de 20 ms para leer después de los rebotes. */

   uc = keyNumber();     /* Determina el número de tecla pulsada */

   if( uc != 0xff)       /* Si solo se ha pulsado una tecla, traduce */
      uc = kbTable[uc];  /* desde kbTable el código asignado a la tecla */

   return(uc);
   }/* FIN DE kbRead */

void leerclave(UCHAR uc, UCHAR *clv)  /*Permite leer la clave desde el teclado*/
{

UCHAR i=0;

	for(i=0;i<4;)	   /*Tras leer cuatro caracteres sale del bucle for, esto
	                     ocurre cuando se ha terminado de introducir toda la 
						 clave*/
	{
	
 	uc=kbRead();	 /*Lee la tecla pulsada y la guarda en la variable uc*/

	clv[i]=uc;		 /*Coloca el caracter guardado en uc en la posición que 
	                   le corresponde del array clv (clave introducida)*/

	if(uc=='*')		 /*En caso de pulsar(*) la secuencia de colocación
					   del caracter leido por el teclado en el array clv
					   comienza de nuevo, poniendo i=0. Esto permite reiniciar 
					   el proceso de lectura en caso de una posible equivocación*/
	 i=0;
	 else i++;	  /*suma 1 a i*/
	
	}
}
