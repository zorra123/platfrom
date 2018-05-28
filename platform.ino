#include "sp.h"
/* right
#define INAR 55     // выходы arduino
#define INBR 56 
#define ENR 54
#define PWMR 2
*/
/* left*/
#define INAL 58     // выходы arduino
#define INBL 59 
#define ENL 57
#define PWML 3

sp Sp;
void setup() {
    Serial.begin(9600); 
   /* pinMode(INAL, OUTPUT);   
    pinMode(INBL, OUTPUT);  
    pinMode(ENL, OUTPUT);   
    digitalWrite(ENL, HIGH);
    pinMode(INAR, OUTPUT);   
    pinMode(INBR, OUTPUT);  
    pinMode(ENR, OUTPUT);   
    digitalWrite(ENR, HIGH);*/
}

void serialEvent() 
{
    if (Serial.available()){
       Sp.sp_Read();                         // Вызов «читалки» принятых данных
       if(Sp.sp_packetAvailable)             // Если после вызова «читалки» пакет полностью принят
       {
        Sp.ParseCommand();                   // Обрабатываем принятую информацию
        Sp.sp_ResetAll();                    // Полный сброс протокола.
       }
    }
}

void loop() {
  
}
