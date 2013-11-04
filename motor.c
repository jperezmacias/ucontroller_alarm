#include <REG2051.H>
//#include <stdio.h>

#define UCHAR unsigned char
#define UINT unsigned int
#include "motor1.h"

UINT TMOTOR; /*si es 1 T=1ms , si es 0 T=2ms  */

enum eSTATE {apagado, encendido,alarma};
extern enum eSTATE state;

void Timer0Init() /*Inicialización del timer que controla el motor */
{
	TMOD&=0xF0;
	TMOD|=0x01;
	TL0=0xFE;
	TH0=0xB7;
	TR0=1; 
	ET0=1;
	EA=1;
	//EX0=0; //No es necesario, está en 0 por defecto.
}

InteTimer0() interrupt 1
{
if (MOTOR==0)
	{	
		MOTOR=1;
			if (TMOTOR==1)
				{TL0=0X66; /*T=1ms*/
				TH0=0XFC;}
			else
				{TL0=0xCC;
				TH0=0xF8;}/*T=2ms*/
	}
else	
	{	TL0=0xFE;
		TH0=0xB7;  /*T=20ms*/
		MOTOR=0;
}

/*El intervalo de 20ms se aprovecha para comprobar los sensores:*/
if(MOTOR==0){
if((state==encendido) && (IR==1 || PULSADOR==0))
{/*Si se encuentra en el estado encendido y alguno de los sensores es activado, 
se enciende el led amarillo y se pasa al estado -atencion alarma- */
state=alarma; 
LR=1;LV=1;LA=0;
//printf("\n%s", "Estado Alarma");
		}	
}	
}
