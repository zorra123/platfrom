#pragma once
#include <Arduino.h>
/* //left*/
#define INAL 55     // выходы arduino
#define INBL 56 
#define ENL 54
#define PWML 2

/* //right*/
#define INAR 58     // выходы arduino
#define INBR 59 
#define ENR 57
#define PWMR 3
class pid
{
public:
	volatile int rpmcountr;
	int rpmr;

	volatile int rpmcountl;
	int rpml;
	
	double Setpointl, Inputl, Outputl;
	double Setpointr, Inputr, Outputr;
	
	pid();
	void compute(byte,byte,byte);
/*private:
	volatile int rpmcount = 0;//see http://arduino.cc/en/Reference/Volatile
	int rpm = 0;
	unsigned long lastmillis = 0;*/
};