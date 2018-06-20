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
	byte Const;			/*константа для вычисления rpm, 
находится след образом: (60/n)*k, где n - кол-во магнитов, у нас их 48.
k - кол-во вызовов за сек, регулируется в основном фале строкой timer_init_ISR_2Hz(),
поскольку частота 2Hz, это 2 вызова в сек.*/

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