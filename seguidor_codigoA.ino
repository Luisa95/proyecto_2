uint8_t kp,ki,kd;//constantes PID, porían ser de 16 bit para mas precisión
 
uint8_t proporcional,prop_anterior,integrativo,derivativo;// control

uint8_t sensor,error,error_anterior,referencia; //      

//D3 adelante D2 atras D 1 derecha D0 izq

void setup() 

{
  error_anterior=prop_anterior=0;
  referencia=0xC0;
DDRC=0xF0; // PC0:3 como entrada, se usan para el ADC
DDRB=0x0F; // PB0:3 como salida, para el pwm 
DDRD=0XF0;//PD4:7 como salida PWM, PD0:3 entradas, para sensores

ADCSRA=(1<<ADEN)|(1<<ADSC);// se habilita el ADC e inicia la conversión

kp=(ADCL); //toma 8 menos significativos retornados en al conversión pin ADC0

ADMUX=(1<<MUX0); // lee pin 1 PIN ADC1

ADCSRA|=(1<<ADSC);

ki=(ADCL); //físicamente como se conecta el aref?

ADMUX&=0x04;// PIN ADC4

ADCSRA|=(1<<ADSC);

kd=(ADCL);

ADCSRA=(0<<ADSC);// termina la conversión

// PD5 OC0B PD6 OC0A PB1 OC1A PB2 OC1B

TCCR0A=0xA2; // Limpia las salidas del pwm OC0A y OC0B cunado el contador alcanza al comparador 10100011 
TCCR0B=(1<<CS00); // completa el modo fast pwm con ocra como vamx y enciende el conteo del 

TCCR1A=0xA0; // Limpia las salidas del pwm OC0A y OC0B cunado el contador alcanza al comparador 10100011 
TCCR1B=(1<<WGM12)|(1<<CS10); // completa el modo fast pwm con ocra como vamx

TCNT0=TCNT1=0;

}

void loop() 

{
  sensor=PIND&0x0F; // 
  
  error=referencia-sensor;
  
  if (sensor==0xC0 || sensor==0x08) 
  {
  OCR0A=OCR0B=255; // los comparadores tienen el máximo valor posible, el pwm tiene el 100% de ciclo util, la velocidad es máxima
  }else

 
     while(sensor==0x04)
     {
     OCR0A=OCR0B=128;  // va mas despacio cuando se sale el sensor puntero 
     }
     while (sensor==0x06); //curva derecha
     {
     proporcional=kp*error;
     integrativo=ki*(proporcional +((error-error_anterior)/2));
     derivativo=kd*(proporcional-prop_anterior);

     prop_anterior=proporcional;
     error_anterior=error;
  
     OCR0B=255;//motor izquiedo
     OCR0A=proporcional+integrativo+derivativo; // motor derecho
     }
     while (sensor==0x05); // curva inzquierda
     {
     proporcional=kp*error;
     integrativo=ki*(proporcional +((error-error_anterior)/2));
     derivativo=kd*(proporcional-prop_anterior);
  
     prop_anterior=proporcional;
     error_anterior=error;

     OCR0A=255; //derecho
     OCR0B=proporcional+integrativo+derivativo; //izquierdo
  
     }
     
 
 
 while (sensor ==0x00);
  {     
     OCR0A=OCR0B=0;
     OCR1A=OCR1B=812;  
  }
while (sensor==0x30);
{
  // por el momento no reconoce el sentido pero se supondra que la esquina superior izquierda sera el sentido donde se mueve la linea
OCR0A=0;
OCR0B=128;  
 }
 /**
 while(!(sensor==0xC0||sensor==0x80||sensor==0x40 || sensor==0x60 || sensor==0x50));
{  
  uint16_t timming=1500;
  OCR0A=OCR0B=0;
  OCR1A=OCR1B=812;  
  delay(timming);
  OCR1A=OCR1B=0;
  OCR0A=128;
  OCR0B=255;  
  delay(timming);
  OCR0A=OCR0B=0;
  OCR1A=OCR1B=812;  
  delay(timming);
  OCR1A=OCR1B=0;
  OCR0B=128;
  OCR0A=255;  
}*/


}
  
