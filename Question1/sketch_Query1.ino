#define Yavaslat PD4
#define Sifirla PD5
int main(void)
{
  int beklemeSuresi=500;
   DDRB = 0xff;   //PORTB output
   PORTB = 0xFF;  //PORTB '0'
   DDRD = 0x00;   //PORTD input
   PORTD = 0x00;  //PORTD internall pullup

   TCCR1B |= (1 << WGM12); // CTC MOD Ayarladık
   TCCR1B |= ((1<<CS12));
   OCR1A=62500;    //counter'ımızın 1 saniyelik değeri
   bool kontrol=false;
   int sure=2;
   int sayac=0;
   while(1)
   {
        if(PIND & (1 << Baslat))//led'ler yanıp sönmeye başlar
        {
        kontrol=true;
        }
        if(PIND & (1 << Durdur)) //led'ler yanıp sönmeyi durdurur
        {
        kontrol=false;
        }
        if(kontrol==true)
        {
            while((TIFR1 & (1<<OCF1A))==0); //her 1 saniyede sayac değişkeninin 1 artırırız
            sayac++;
            if(sayac==sure)  // verdiğimiz default süreyle sayac eşit olursa led'lerin tersi alınır
              {
                  PORTB = ~PORTB;
                  sayac=0;
              }
              if(PIND & (1 << Hizlandir))//hızlandır butonuna basılınca sure azalır ve ledler daha hızlı yanıp söner
              {
                if (sure>1){
                  sure=sure-1;
                }
              }
              if(PIND & (1 << Yavaslat))//yavaslat butonu süreyi artır ve ledler daha yavaş yanıp söner
              {
                  sure=sure+1;
              }
              if(PIND & (1 << Sifirla))//sıfırla butonumuz led'ler hangi hızda yanarsa yansın onları 5 sn yapar
              {
                  sure=5;
              }
              TIFR1 |= (1 << OCF1A);
        }
      if(kontrol==false)
      {
      }
   }
}
