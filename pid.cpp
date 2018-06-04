#include "pid.h"

pid::pid()
{
	Setpointl = 4;
	Inputl = 25;
	rpmcountl = 0;
	rpml = 0;
	
	Setpointr = 4;
	Inputr = 18;
	rpmcountr = 0;
	rpmr = 0;
}
void pid::compute(byte id, byte instr, byte par)
{
  
	switch(id){
		case 1://на левый движок
			switch(instr){
				case 1:
					Serial.println("ok");
					
					digitalWrite(INAL, LOW);
					digitalWrite(INBL, HIGH); 
					digitalWrite(ENL, HIGH);
					Setpointl = par;
				break;
			}
		
		break;
		
		case 2://на правый движок
			switch(instr){
				case 1:
					digitalWrite(INAR, LOW);
					digitalWrite(INBR, HIGH); 
					digitalWrite(ENR, HIGH);
					
					Setpointr = par;
				break;
			}
		
		break;
		case 3://на все движки
			switch(instr){
				case 1://вперед
					digitalWrite(INAR, HIGH);
					digitalWrite(INBR, LOW); 
					digitalWrite(ENR, HIGH);
					Setpointl = par;
					
					digitalWrite(INAL, HIGH); 
					digitalWrite(INBL, LOW); 
					digitalWrite(ENL, HIGH);
					Setpointr = par;
				break;
				case 2://назад
					digitalWrite(INAR, LOW);
					digitalWrite(INBR, HIGH); 
					digitalWrite(ENR, HIGH);
					Setpointl = par;
					
					digitalWrite(INAL, HIGH); 
					digitalWrite(INBL, LOW); 
					digitalWrite(ENL, HIGH);
					Setpointr = par;
				break;
				case 3://стоп
					digitalWrite(INAR, LOW);    
					digitalWrite(INBR, LOW); 
					digitalWrite(ENR, LOW);
					Setpointr = 0;
					
					digitalWrite(INAL, LOW);     
					digitalWrite(INBL, LOW); 
					digitalWrite(ENL, LOW);
					Setpointl = 0;
				break;
			}
		break;
	}
	
}
