#include <LiquidCrystal.h> //Dołączenie bilbioteki
#include <Wire.h>
#include "RTClib.h"
#include "PCF8574.h"
RTC_DS1307 RTC;
PCF8574 expander;
PCF8574 expander2;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
struct kwiaty {
  String nazwa;
  int ostdzien;
  int ostmiesiac;
  int ostrok;
  int ostgodzina;
  int ostminuty;
  int nastdzien;
  int nastmiesiac;
  int nastrok;
  int nastgodzina;
  int nastminuty;
};
const int n=4;
kwiaty kw[n];
void setup() {
  Serial.begin(9600);
  Wire.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  RTC.begin();
    if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
   RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  expander.begin(0x20);
  expander2.begin(0x21);
 // expander.pinMode(7,INPUT);
  expander.pinMode(0,OUTPUT);
  expander.pinMode(1,OUTPUT);
  expander.pinMode(2,OUTPUT);
  expander.pinMode(3,OUTPUT);
  expander.pinMode(4,OUTPUT);
  expander.pinMode(5,OUTPUT);
  expander.pinMode(6,OUTPUT);
  expander.pinMode(7,OUTPUT);
  expander2.pinMode(0,OUTPUT);
  expander2.pinMode(1,OUTPUT);
  //expander.pullUp(7);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
 load_flowers();

  lcd.begin(16,2); 
}
int next=0;
boolean point=0;
void loop() {
  DateTime now = RTC.now();
       expander.digitalWrite(0,HIGH);
       expander.digitalWrite(1,HIGH);
       expander.digitalWrite(2,HIGH);
       expander.digitalWrite(3,HIGH);
       expander.digitalWrite(4,LOW);
       expander.digitalWrite(5,LOW);
       expander.digitalWrite(6,LOW);
       expander.digitalWrite(7,LOW);
       expander2.digitalWrite(0,HIGH);
       expander2.digitalWrite(1,LOW);
       
    if(point==0)
    {
      /*sprawdz(now);*/
      lcd.clear();
      actualtime(now);
       if(digitalRead(8)==LOW)
        {
           point=!point;
        }
    }
   else{
        //sprawdz(now);
        if(digitalRead(9)==LOW)
        {
          lcd.clear();
          next++;
        }
       for(int curs=0;curs<30;curs++)
        {  
            print_flower(next);
            lcd.scrollDisplayLeft();
            if(digitalRead(9)==LOW)
            {
              next++;             
              lcd.clear();
            }
            else if(digitalRead(8)==LOW){
              point=0;
              break;
            }
           if(next>n-1)
                  next=0;  
           delay(1000);
        }
    }
   
  delay(500);
}/*
void przesun(int ktory)
{
    kw[ktory].nastdzien=1;
    kw[ktory].nastmiesiac++;
}
void miesiace(int ktory)
{
  switch(kw[ktory].nastmiesiac)
  {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    if(kw[ktory].nastdzien>31)
      przesun(ktory);
    break;
    case 4:
    case 6:
    case 9:
    case 11:
    if(kw[ktory].nastdzien>30)
          przesun(ktory);
    break;
    case 2:
    if(kw[ktory].nastdzien>28)
       przesun(ktory);
    break;
    case 12:
    if(kw[ktory].nastdzien>31)
    {
      kw[ktory].nastdzien=1;
      kw[ktory].nastmiesiac=1;
      kw[ktory].nastrok++;
    }
    break;
  }
}

void sprawdz(DateTime now)
{
  
  for(int i=0;i<n;i++)
  {
    if(kw[i].nastrok<=now.year() && kw[i].nastmiesiac<=now.month() && kw[i].nastdzien<=now.day() && kw[i].nastgodzina<=now.hour() && kw[i].nastminuty<=now.minute())
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Prosze czekac");
      lcd.setCursor(0, 1);
      lcd.print("kwiatek "+kw[i].nazwa);
     /* expander.digitalWrite(i,LOW);
      expander.digitalWrite(i+n,HIGH);*/
      /*kw[i].ostdzien=kw[i].nastdzien;
      kw[i].ostmiesiac=kw[i].nastmiesiac;
      kw[i].ostrok=kw[i].nastrok;
      kw[i].ostgodzina=kw[i].nastgodzina;
      kw[i].ostminuty=kw[i].nastminuty;
      kw[i].nastminuty+=5;
      if(kw[i].nastminuty>59)
        {
          kw[i].nastminuty=0;
          kw[i].nastgodzina++;
        }
      if(kw[i].nastgodzina>23)
      {
        kw[i].nastgodzina=0;
        kw[i].nastdzien++;
      }
      miesiace(i);
      delay(5000);
      /*expander2.digitalWrite(i,HIGH);
      expander2.digitalWrite(i+n,LOW);*/
  /*  }
  }
}*/
void actualtime(DateTime now)
{
 
  lcd.setCursor(0, 0);
  lcd.print(now.year(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  
  lcd.setCursor(0,1);

  LCDprint(now.hour());
  Serial.print(now.hour());
  Serial.print(":");
  lcd.print(':');
  LCDprint(now.minute());
  Serial.print(now.minute());
  Serial.print(":");
  lcd.print(':');
  LCDprint(now.second());
  Serial.print(now.second());
  Serial.print("\n");
  lcd.print(" ");/*
  if(now.hour()<12)
   lcd.print("AM");
   else
    lcd.print("PM");*/
    
}
          

void print_flower(int num)
{
  lcd.setCursor(0, 0);
  lcd.print(kw[num].nazwa);
  lcd.print(" ");
  lcd.print("ost.podl.");
  lcd.print(":");
  lcd.print(kw[num].ostrok);
  lcd.print("/");
  lcd.print(kw[num].ostmiesiac);
  lcd.print("/");
  lcd.print(kw[num].ostdzien);
  lcd.print(" ");
  lcd.print(kw[num].ostgodzina);
  lcd.print(":");
  LCDprint(kw[num].ostminuty);
  lcd.setCursor(0, 1);
  lcd.print("nast. podl.");
  lcd.print(":");
  lcd.print(kw[num].nastrok);
  lcd.print("/");
  lcd.print(kw[num].nastmiesiac);
  lcd.print("/");
  lcd.print(kw[num].nastdzien);
  lcd.print(" ");
  lcd.print(kw[num].nastgodzina);
  lcd.print(":");
  LCDprint(kw[num].nastminuty);
}
void load_flowers()
{
  kw[0].nazwa = "celozja";
  kw[1].nazwa = "kaktus";
  kw[2].nazwa = "roza";
  kw[3].nazwa="proba";
  int kolejne=0;
  for(int i=0;i<n;i++){
   kw[i].ostrok = 2017;
   kw[i].ostmiesiac = 5;
   kw[i].ostdzien = 31;
   kw[i].ostgodzina = 8;
   kw[i].ostminuty = 0;
   kw[i].nastrok = 2017;
   kw[i].nastmiesiac = 5;
   kw[i].nastdzien = 31;
   kw[i].nastgodzina = 17;
   kw[i].nastminuty = 45+kolejne;
   kolejne++;
  }

}
void LCDprint(int number)
{
  if (number >= 0 && number < 10)
    lcd.write('0');
  lcd.print(number);
}
