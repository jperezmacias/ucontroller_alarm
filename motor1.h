
sbit MOTOR = P3^7;//si es 0 activamos el motor
extern UINT TMOTOR; //si es 1 T=1ms, si es 0 T=2ms 
sbit LV = P3^5; //LED VERDE --ALARMA DESACTIVADA-
sbit LR = P3^3; //LED ROJO --ALARMA ACTIVADA-
sbit LA = P3^4; //LED AMARILLO --ATENCION ALARMA-
sbit PULSADOR=P3^0; //SENSOR PRESION
sbit IR = P3^1; //SENSOR IR
void Timer0Init(); //Inicialización del timer

