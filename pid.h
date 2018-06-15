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
	double Setpointl, Inputl, Outputl;
	double Setpointr, Inputr, Outputr;
	
	bool flagl;	
	bool flagr;
private:
	volatile int rpmcountr;
	int rpmr,rpmrlast;
	
	volatile int rpmcountl;
	int rpml,rpmllast;
public:
	pid();
	void initialization(byte,byte,byte);
	void computeSpeed(byte *);
	//void print(byte *);
	void rpm_fanl();
	void rpm_fanr();
	void write(byte);
};