#include "sp.h"
#include"timer-api.h"
#include <PID_v1.h>
//volatile int rpmcountl = 0;
unsigned long lastmillis = 0;
sp Sp;
pid pid;
//double Outputl,Inputl,Setpointl;
PID myPIDL(&pid.Inputl, &pid.Outputl, &pid.Setpointl,0.4, 7, 0, DIRECT);//создаем ПИД-регулятор
PID myPIDR(&pid.Inputr, &pid.Outputr, &pid.Setpointr,0.4, 8, 0, DIRECT);//создаем ПИД-регулятор
					
void setup() {
    Serial.begin(9600); 
	pinMode(INAR, OUTPUT);   
	pinMode(INBR, OUTPUT);  
	pinMode(ENR, OUTPUT);
	
	pinMode(INAL, OUTPUT);   
	pinMode(INBL, OUTPUT);  
	pinMode(ENL, OUTPUT);
	
	digitalWrite(INAR, HIGH);
	digitalWrite(INBR, LOW); 
	digitalWrite(ENR, HIGH); 
	
	digitalWrite(INAL, LOW); 
	digitalWrite(INBL, HIGH); 
	digitalWrite(ENL, HIGH);
	
	analogWrite(PWML, pid.Inputl);
	analogWrite(PWMR, pid.Inputr); 
	
	attachInterrupt(2, rpm_fanl, FALLING);
	attachInterrupt(3, rpm_fanl, FALLING);
	attachInterrupt(4, rpm_fanr, FALLING);
	attachInterrupt(5, rpm_fanr, FALLING); 
	
	myPIDL.SetMode(AUTOMATIC);
	myPIDR.SetMode(AUTOMATIC);

	
    timer_init_ISR_2Hz(TIMER_DEFAULT);
}
void timer_handle_interrupts(int timer) {
	
	detachInterrupt(2);
	detachInterrupt(3);
	detachInterrupt(4);
	detachInterrupt(5);
    static unsigned long prev_time = 0;
    
    unsigned long _time = micros();
    unsigned long _period = _time - prev_time;
    prev_time = _time;	

	pid.computeSpeed();
	byte rpm[2];
	pid.print(rpm);
	
	Sp.Send(rpm);
		
	attachInterrupt(2, rpm_fanl, FALLING);
	attachInterrupt(3, rpm_fanl, FALLING);
	attachInterrupt(4, rpm_fanr, FALLING);
	attachInterrupt(5, rpm_fanr, FALLING);

	if(pid.flagl == 1&&pid.flagr == 1)// пид работает для двух двигателей
	{
		if(myPIDL.Compute() && myPIDR.Compute())//считаем выходной сигнал ПИД-регулятора
		{
			pid.write(3);
			/*lastmillis = millis(); // Uptade lasmillis
			Serial.print("pid.Inputl ");
			Serial.print(rpm[0]);
			Serial.print('\t');		
			Serial.print("pid.Inputr ");	
			Serial.println(rpm[1]);*/
		}	
	}  
	else if(pid.flagl == 1&&pid.flagr == 0)// пид работает для левого двигателя
	{
		if(myPIDL.Compute())//считаем выходной сигнал ПИД-регулятора
		{
			pid.write(1);
			/*lastmillis = millis(); // Uptade lasmillis
			Serial.print("pid.Inputl ");
			Serial.print(rpm[0]);
			Serial.print('\t');		
			Serial.print("pid.Inputr ");	
			Serial.println(rpm[1]);*/
		}
	}
	else if(pid.flagl == 0&&pid.flagr == 1)// пид работает для правого двигателя
	{
		if(myPIDR.Compute())//считаем выходной сигнал ПИД-регулятора
		{
			pid.write(2);
			/*lastmillis = millis(); // Uptade lasmillis
			Serial.print("pid.Inputl ");
			Serial.print(rpm[0]);
			Serial.print('\t');		
			Serial.print("pid.Inputr ");	
			Serial.println(rpm[1]);*/
		}
	}
}
void rpm_fanl()
{ 
	pid.rpm_fanl();
}
void rpm_fanr() 
{ 
	pid.rpm_fanr();
}
void serialEvent() 
{
    if (Serial.available()){
  		Sp.sp_Read();                         // Вызов «читалки» принятых данных
  		if(Sp.sp_packetAvailable)             // Если после вызова «читалки» пакет полностью принят
  		{
  			Sp.ParseCommand(pid);                   // Обрабатываем принятую информацию
  			Sp.sp_ResetAll();                    // Полный сброс протокола.
  		}
    }
}

void loop() {	
}
