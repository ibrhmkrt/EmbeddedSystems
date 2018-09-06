
#include <avr/io.h>
#include <util/delay.h>
#include <LiquidCrystal.h>
#include <Servo.h>
Servo myservo;
char charBuff[10];//ifadeleri asagidaki tanimli fonksiyonlarda okumak icin gecici array
char charBuff1[10];

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte customChar[8] = {//ozel karakter belirttim süsleme derece simgesi
  0b00000,
  0b01110,
  0b01010,
  0b01110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};


void SendCommand(unsigned char);//cursor belirleme
void SendData(unsigned char);//lcd ye gonderilen data kismi
void SendString(const char*);//okunacak ifade
void TransferByteIn4BitMode(unsigned char);//gonderim
void TransferHighNibble(unsigned char);//yazim


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
void InitADC2()//diger init ile ayni mantik
{
  ADMUX = (1 << REFS0);
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //Rrescalar div factor =128
}


uint16_t ReadADC2(uint8_t ch2)//diger adc ile ayni mantik.
{
  ch2 &= 0b00000111;
  ADMUX = (ADMUX & 0xF8) | ch2;
  ADCSRA |= (1 << ADSC);
  while (!(ADCSRA & (1 << ADIF)));
  ADCSRA |= (1 << ADIF);

  return ADC;
}



int main(void)//setup kismi
{
 
  lcd.begin(16, 2);//lcd tanimladik.
  lcd.createChar(0, customChar);//ozel karakter girdisi.(hazir fonksiyon ama susleme amacli)
  SendCommand(0x83);//cursoru 4.sutuna getirdik.
  lcd.write(byte(0));//derece ifademizi yazdirdik.
  uint16_t adc_result;//degisken(A0)
  uint16_t adc_result2;//degisken(A1) icin
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); //cevrilmis PWM
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10); //Prescaler=64 ayarlandi

  ICR1 = 4999; //PWM frenkansi=50Hz Periyodu = 20ms ayarlandi

  DDRB |= (1 << 1);//servo pini ayarlandi.

  InitADC();//A0 icin adc fonksiyonu cagirildi.
  InitADC2();//A1 icin adc fonksiyonu cagirildi
  OCR1A = 130;//servo basi baslangic konumundan baslatildi.




  int derece = 0;//servo kac derece dondu gormek icin.
  int derece2 = 0;
  int reelderece=0;
  int reelaci=0;
  while (1)//loop kismi
  {
    adc_result = ReadADC(0);//A0 pinini okuduk
    adc_result2 = ReadADC(1);
    derece = adc_result;
    derece = derece * 0.20;//0.20 nedeni analog okunan degeri yaklasik hesap ile derece gibi lcd ye yansitmak

    derece2 = adc_result2;
    derece2= derece2 * 0.20;

    if (derece>derece2)
    {
      reelderece=derece;
      reelaci=adc_result;
    }
      
    else
    {
      reelderece=derece2;
      reelaci=adc_result2;
    }
      
      
    sprintf(charBuff, "%d", reelderece);//dereyi charBuff a almak icin.
    OCR1A = reelaci;//okunan analog degere(A0) gore servo hareketi.
    _delay_ms(200);
    
 
    SendCommand(0x80);//cursoru baslangic konumuna getirdik
    SendString(charBuff);//ekrana yazdirdik.


    if ((derece < 100))//3 basamakli ifadeden 2 basamaga indiginde yazimda hata olmamasi icin
    {
      lcd.clear();//lcd temizleyip 2 basamaksa 2 -3basamaksa 3 basamak ayarlamak icin.
      SendString(charBuff);
      SendCommand(0x83);//tekrar dereceyi yazdirdik.
      _delay_ms(20);//20ms bekleme duzgun islem icin.
    }
  }
  return 0;
}
void SendCommand(unsigned char cmd)
{

  PORTB &= ~(1 << 4); //RS<-0//cursor fonksiyonu
  TransferByteIn4BitMode(cmd);
}
void SendData(unsigned char dt)
{

  PORTB |= (1 << 4); //RS<-1//veri fonksiyonu
  TransferByteIn4BitMode(dt);
}
void SendString(const char *cmdPtr)
{
  for (unsigned int i = 0; cmdPtr[i] != '\0'; i++) //char yazdirmamiz.
    SendData(cmdPtr[i]);
}
void TransferByteIn4BitMode(unsigned char byt)
{
  TransferHighNibble(byt);

  PORTB |= (1 << 3);
  PORTB &= ~(1 << 3);

  TransferHighNibble(byt << 4);
  PORTB |= (1 << 3);
  PORTB &= ~(1 << 3);

}
void TransferHighNibble(unsigned char byt)
{
  unsigned char temp;
  temp = (byt >> 7) & 1;
  digitalWrite(2, temp);

  temp = (byt >> 6) & 1;

  digitalWrite(3, temp);
  temp = (byt >> 5) & 1;
  digitalWrite(4, temp);

  temp = (byt >> 4) & 1;

  digitalWrite(5, temp);
}
