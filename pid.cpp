#include "pid.h"

pid::pid()
{
	Setpointl = 6;
	Inputl = 0;
	rpmcountl = 0;
	rpml = 0;
	flagl = 1;
	rpmllast = 0;
	
	Setpointr = 6;
	Inputr = 0;
	rpmcountr = 0;
	rpmr = 0;
	flagr = 1;
	rpmrlast = 0;
	
	Const = (60/48)*2;
}
void pid::rpm_fanl()
{
	rpmcountl++;
}
void pid::rpm_fanr()
{
	rpmcountr++;
}
void pid::computeSpeed(byte *rpm)
{
	rpmllast = rpml;
	rpml = rpmcountl * Const;
	rpmcountl = 0;
	
	rpmrlast = rpmr;
	rpmr = rpmcountr * Const;
	rpmcountr = 0;
	
	Inputl = rpml;
	Inputr = rpmr;
	
	rpm[0] = Inputl;
	rpm[1] = Inputr;
	
	//if(rpml == Setpointl&&(rpml-rpmllast<1||rpml-rpmllast>-1)) flagl = 0;
	//if(rpmr == Setpointr&&(rpmr-rpmrlast<1||rpmr-rpmrlast>-1)) flagr = 0;
}
/*void pid::print(byte *rpm)
{
	rpm[0] = Inputl;
	rpm[1] = Inputr;
}*/
void pid::write(byte par)
{
	switch (par){
		case 1:			
			analogWrite(PWML,Outputl);
			rpmcountl = 0;
		break;
		case 2:
			analogWrite(PWML,Outputr);
			rpmcountr = 0;
		break;
		case 3:
			analogWrite(PWML,Outputl);
			analogWrite(PWMR,Outputr);
			rpmcountl = 0; // Restart the RPM counter
			rpmcountr = 0; // Restart the RPM counter
		break;
	}
}
void pid::initialization(byte id, byte instr, byte par)
{
	switch(id){
		case 1://на левый движок
			switch(instr){
				case 1://вперед
					//Serial.println("ok");
					
					digitalWrite(INAL, LOW);
					digitalWrite(INBL, HIGH); 
					digitalWrite(ENL, HIGH);
					
					Setpointl = par;
					flagl =1;
				break;
				case 2://назад
					//Serial.println("ok");
					
					digitalWrite(INAL, HIGH);
					digitalWrite(INBL, LOW); 
					digitalWrite(ENL, HIGH);
					
					Setpointl = par;
					flagl =1;
				break;
				case 3://стоп
					//Serial.println("ok");
					
					digitalWrite(INAL, LOW);
					digitalWrite(INBL, LOW); 
					digitalWrite(ENL, LOW);
					
					Setpointl = 0;
					flagl =0;
				break;
			}
		
		break;
		
		case 2://на правый движок
			switch(instr){
				case 1:
					digitalWrite(INAR, HIGH);
					digitalWrite(INBR, LOW); 
					digitalWrite(ENR, HIGH);
					
					Setpointr = par;
					flagr = 1;
				break;
				case 2:
					digitalWrite(INAR, LOW);
					digitalWrite(INBR, HIGH); 
					digitalWrite(ENR, HIGH);
					
					Setpointr = par;
					flagr = 1;
				break;
				case 3:
					digitalWrite(INAR, LOW);
					digitalWrite(INBR, LOW); 
					digitalWrite(ENR, LOW);
					
					Setpointr = 0;
					flagr = 0;
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
					flagl = 1;
					
					digitalWrite(INAL, LOW); 
					digitalWrite(INBL, HIGH); 
					digitalWrite(ENL, HIGH);
					
					Setpointr = par;
					flagr = 1;
				break;
				case 2://назад
					digitalWrite(INAR, LOW);
					digitalWrite(INBR, HIGH); 
					digitalWrite(ENR, HIGH);
					
					Setpointl = par;
					flagl = 1;
					
					digitalWrite(INAL, HIGH); 
					digitalWrite(INBL, LOW); 
					digitalWrite(ENL, HIGH);
					
					Setpointr = par;
					flagr =1;
				break;
				case 3://стоп
					
					digitalWrite(INAL, LOW);     
					digitalWrite(INBL, LOW); 
					digitalWrite(ENL, LOW);
					
					Setpointl = 0;
					flagl = 0;
					
					digitalWrite(INAR, LOW);    
					digitalWrite(INBR, LOW); 
					digitalWrite(ENR, LOW);
					
					Setpointr = 0;
					flagr = 0;
				break;
			}
		break;
	}
}
