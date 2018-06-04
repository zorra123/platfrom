#include "sp.h"
#include <PID_v1.h>
volatile int rpmcountl = 0;
unsigned long lastmillis = 0;
sp Sp;
pid pid;
double Outputl,Inputl,Setpointl;
PID myPIDL(&pid.Inputl, &pid.Outputl, &pid.Setpointl,0.5, 25, 0, DIRECT);//создаем ПИД-регулятор
PID myPIDR(&pid.Inputr, &pid.Outputr, &pid.Setpointr,0.4, 16, 0, DIRECT);//создаем ПИД-регулятор
					
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
}
void rpm_fanl()
{ 
	pid.rpmcountl++;
}
void rpm_fanr() 
{ 
	pid.rpmcountr++;
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
	if(millis() - lastmillis >= 1000)
	{
		detachInterrupt(2);
		detachInterrupt(3);
		detachInterrupt(4);
		detachInterrupt(5);
		
		pid.rpml = pid.rpmcountl * 1.25;
		pid.rpmcountl = 0;
		
		pid.rpmr = pid.rpmcountr * 1.25;
		pid.rpmcountr = 0;

		attachInterrupt(2, rpm_fanl, FALLING);
		attachInterrupt(3, rpm_fanl, FALLING);
		attachInterrupt(4, rpm_fanr, FALLING);
		attachInterrupt(5, rpm_fanr, FALLING);

		pid.Inputl = pid.rpml;
		pid.Inputr = pid.rpmr;
		
		if(pid.Setpointl!=4||pid.Setpointr!=4)
		{
			if(myPIDL.Compute() && myPIDR.Compute())//считаем выходной сигнал ПИД-регулятора
			{
				analogWrite(PWMR,pid.Outputr);
				analogWrite(PWML,pid.Outputl);
				pid.rpmcountl = 0; // Restart the RPM counter
				pid.rpmcountr = 0; // Restart the RPM counter
				lastmillis = millis(); // Uptade lasmillis
				Serial.print("pid.Inputl ");
				Serial.print(pid.Inputl);
				Serial.print('\t');		
				Serial.print("pid.Inputr ");	
				Serial.println(pid.Inputr);	
			}	
		}
	}
  
}
