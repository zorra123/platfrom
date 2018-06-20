#pragma once
#include "String.h"
#include "pid.h"
#include <Arduino.h>
class sp
{
public:
	boolean sp_packetAvailable;		// Флаг завершения приема пакета
private:	
	String sp_dataString;			// Здесь будут храниться принимаемые данные
	byte id;						//	распаршенные данные id instr par
	byte instr;						//
	byte par;						//
	String sp_startMarker;			// Переменная, содержащая маркер начала пакета
	String sp_stopMarker;			// Переменная, содержащая маркер конца пакета
	int sp_startMarkerStatus;		// Флаг состояния маркера начала пакета
	int sp_stopMarkerStatus;		// Флаг состояния маркера конца пакета
	unsigned int sp_dataLength;		// Флаг состояния принимаемых данных
	
public:
	sp();
	void ParseCommand(pid&); 	//парсинг принимаемого пакета
	void sp_ResetAll();			//сброс всех параметров, необходимых для считывания пакета
	void sp_Read();				//чтение пакета
	void Send(byte *);			//отправка пакета, содержащего текующие ppm колес
	
private:
	//void sp_Reset();				
	unsigned int hexToDec(String);	//перевод из 16 в 10 систему счисления
	String decToHex(byte);			//перевод из 10 в 16 систему счисления

	
};
