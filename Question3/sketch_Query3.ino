#include<avr/io.h>
#include <stdint.h>

#define BLUE     _BV(PB0)
#define BLACK    _BV(PB1)
#define RED      _BV(PB2)
#define YELLOW   _BV(PB3)


void InitADC()
{
  ADMUX |= (1 << REFS0);//okunan analog degeri adc ye cevirme
  ADCSRA |= (1 << ADEN) | (1 << ADSC);//adc yi actik
}


uint16_t ReadADC(uint8_t ch)
{

  ADMUX &= 0xF0;//daha once okunan kanali temizledi
  ADMUX |= ch;//analog pin okuma icin yeni adc kanali tanimlandi.
  ADCSRA |= (1 << ADSC);//donusum
  ADCSRA |= (1 << ADSC);//donusum
  while ((ADCSRA & (1 << ADSC)));//donusum gerceklesene kadar bekle.

  return ADCW;//adc degerini dondur
}




int main(void)
{
   DDRB = 0xff;   //PORTB output
   PORTB = 0x00;  //PORTB '0'
   InitADC();
   TCCR1B |= (1 << WGM12);
   TCCR1B |= ((1<<CS12));
   OCR1A=62500;
   int sayac=0;
   uint16_t ldr;
   while(1)
   {
    ldr = ReadADC(0);
    if(ldr<=20)
    {
     while((TIFR1 & (1<<OCF1A))==0);
          sayac++;
          if(sayac<=4)
          {
            if(sayac>0 && sayac <=1)
              PORTB = _BV(PB0);
            if(sayac>1 && sayac <=2)
              PORTB = BLACK;
            if(sayac>2 && sayac <=3)
              PORTB = RED;
            if(sayac>3 && sayac <=4)
              PORTB = YELLOW;
            //4 saniye boyunca saat yönünde dönecek;
          }
          
          if(sayac>4 && sayac <=6)
          {
            
          }
          
          if(sayac>6 && sayac <=10)
          {
            if(sayac>6 && sayac <=7)
              PORTB = YELLOW;
            if(sayac>7 && sayac <=8)
              PORTB = RED;
            if(sayac>8 && sayac <=9)
              PORTB = BLACK;
            if(sayac>9 && sayac <=10)
              PORTB = BLUE;
          }
          
          if(sayac>10 && sayac <=12)
          {
            //2 saniye bekleyecek
          }
          
          if (sayac>12)
          {
            sayac=0;
          }
          
          TIFR1 |= (1 << OCF1A);
    }
    else
    {
      
    }
    
  }
}
