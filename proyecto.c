#define UCHAR unsigned char
#define UINT unsigned int

#include <REG2051.H>
#include <motor1.h>
#include <teclado1.h>

// Conexiones del teclado al P1
//    7  6  5  4    3  2  1  0
//   NU-c1-f1-c0   f4-c2-f3-f2
//		 2		1	   3
 

void teclado(void);
void inicioprograma(void);

void comparar(UCHAR *clv);
void Timer0Init(void); 
UINT teclado(void);

enum eSTATE {apagado, encendido,alarma};
enum eSTATE state = apagado;
enum eSTATE oldstate = apagado;
UCHAR clave[6];
UCHAR OK;


void main(void)
 {
   UCHAR uc=0xff;
   kbInit();     /* Inicializa el teclado */
  	Timer0Init();	/* Inicializar el temporizador 0 para el motor*/
	MOTOR=0;   		/*Activación del motor*/
	
   while(1)					/*Hace que el programa se repita*/
      {
	inicioprograma();	  /*Llamamos a nuestro programa desde el main*/
		
 
     
   	}
	/* FIN DE main */

}
/*******************************************************
  void kbInit(void)
  Inicia el teclado para que pueda detectar pulsaciones.
  (leyendo las filas) 
********************************************************/


void inicioprograma(void)  /*Programa cerebro de la alarma, gestiona los
							estados y nos permite conmutar entre ellos*/
{
UINT p=0;
	switch(state)	   /*Miramos variable state para cambiar de estado*/
	
	{
		
		case encendido:     /*ESTADO -ALARMA ENCENDIDA-
							Led rojo y puerta cerrada impiden el paso, 
							los sensores se encuentran activos. En caso de 
							saltar alguno de los dos se cambia state a 
							-atencion alarma-, pero esto se controla mediante 
							una interrupcion*/
		
			   OK=2;   	 /*Evita posibles conflictos en caso de que OK
						 tuviese un 1 o un 0 en algun estado anterior, ya que la
						 variable OK es usada de forma global en todos los
						 estados*/
			
				TMOTOR=0; /*T=2ms. Motor gira a 0 grados, manteniendo la puerta
							cerrada*/
			
				LR=0;LV=1;LA=1; /*Se enciend el led rojo y se apagan los demas
				                (NOTA: los leds se encienden en 'low' tal
								 y como se puede observar)*/
			
				//printf("\n%s", "Estado encendido");
			   	/*if(IR==1 || PULSADOR==1)        
					  	{state=alarma;
						 break;}*/

						teclado();	 /*Llamada a la lectura de clave*/
						while(kbHit() != 0);   /*Efectúa una pausa hasta que se
												suelte la tecla para evitar
												repeticiones de lectura*/

						if(OK=='0')    /*Si la lectura de clave es incorreta
										se activan las medidas de seguridad
										pasando al estado -atencion alarma-, 
										este codigo evita la posibilidad de
										probar claves de forma malintencionada 
										manteniendo la seguridad al maximo nivel*/  
	                      
						  	state=alarma;
						  
						if(OK=='1')		 /*Si la lectura de clave es correcta
											habremos conseguido desactivar la
											alarma, obligando a state a pasar
											del estado -alarma encendida- al 
											estado -alarma apagada-*/
							state=apagado;
						
						
			 			
			break;	/*Pertenece al propio switch*/

		case apagado:	    /*ESTADO -ALARMA APAGADA-
							Led verde y puerta abierta permiten el paso, 
							los sensores se encuentran desactivados*/

				OK=0;	/*Mantiene a la alarma en el estado apagado,
						evitando posibles conflictos en caso de que OK
						tuviese un 1 en algun estado anterior, ya que la
						variable OK es usada de forma global en todos los
						estados*/
				     
				TMOTOR=1; /*T=2ms. Motor gira 90 grados, abriendo la puerta*/
				LR=1;LV=0;LA=1;	 /*Se enciende led verde y se apagan los demas*/
				//	printf("\n%s", "Estado apagado");
				teclado();	/*Llamada a la lectura de clave*/
				while(kbHit() != 0);	/*Efectúa una pausa hasta que se
										suelte la tecla para evitar
										repeticiones de lectura*/
				
				if(OK=='1')		    /*Si la lectura de clave es correcta
									habremos conseguido activar la alarma, 
									obligando a state a pasar al estado 
									encendido, en caso de ser incorrecta
									permaneceremos en el estado alarma
									apagada*/
					state=encendido;
				break;
	
		case alarma:	  /*ESTADO -ATENCION ¡¡ALARMA!!-
							Led amarillo indica que la alarma ha saltado,
							esto ocurre cuando, estando en el estado -alarma
							encendida-, la clave es introducida incorrectamente
							o salta alguno de los dos sensores*/ 
							
				OK=0;   /*Mantiene a la alarma en el estado -atencion alarma-,
						evitando posibles conflictos en caso de que OK
						tuviese un 1 en algun estado anterior, ya que la
						variable OK es usada de forma global en todos los
						estados*/
			
				TMOTOR=0; /*Mantiene la puerta cerrada*/
				//led amarillo de alarma
				LR=1;LV=1;LA=0;	/*Se enciende led amarillo y se apagan los demas*/
				//printf("\n%s", "Estado Alarma");
				teclado();	/*Llamada a la lectura de clave*/
				while(kbHit() != 0);    /*Efectúa una pausa hasta que se
										suelte la tecla para evitar
										repeticiones de lectura*/
				if(OK=='1')	    	/*Si la lectura de clave es correcta
									habremos conseguido desactivar el estado 
									-atencion alarma-, obligando a state a pasar 
									al -estado apagado-, en caso de ser 
									incorrecta permaneceremos en el estado 
									-atencion alarma-*/
					state=apagado;
			
		}

        
	if(oldstate != state){	   /*Permite actualizar los estados*/
		oldstate = state;
			}
	
}

void teclado(void)		  /*Funcion que controla la lectura de la clave*/
{
UCHAR uc=0xff;		


uc=kbRead();
//printf("%c", uc);	 //Si se ha pulsado una tecla enviala a través del puerto serie
		      if(uc != 0xff)  /*Controla que el caracter que se este leyendo no
			                    sea nulo*/
				{
					 if (uc=='*')  /*Antes de entrar en el proceso de lectura
					 				de clave se debe pulsar (*) */
						{	
						
						UINT i=0;    
						//LR=0;LV=0;LA=1;
							leerclave(uc,&clave); /*Lamada a leer la clave*/
							clave[4] = 0;  /*La clave esta declarada
											como un array de 4 posiciones*/
							//printf("\n%s", clave);
							comparar(&clave); /*Llamada a comparar la clave
												introducida con la clave valida*/ 													
						  	//printf("\n%s", "Ok=");
							//printf("%c",OK);
						}
			        
				 
		      	}
}


	

void comparar(UCHAR *clv)  /*Compara clave introducida con la clave valida*/
{
UCHAR usuario[4] = { '1', '4', '7', '2'};  /*Clave valida*/
UCHAR i=0;
		for (i=0;i<4;i++)  /*Contador para ir comparando cada posicion
							 de la clave*/
			{	
		
			if (clv[i]!=usuario[i])	/*Compara la clave introducida
									  con la clave valida. Si alguna de 
									  las posiciones no coincide se sale del 
									  if poniendo i=0. Si la clave es valida
									  i tendrá el valor 4 */
										
				{i=0;break;}
		
		}

if(i==4) 	/*Si la clave introducida es valida OK=1*/
	OK='1'; 
else OK='0'; /*Si la clave introducida es invalida OK=0*/
/*devuelve un cero si es incorrecto y un 1 si es correcto en la variable OK*/
}


