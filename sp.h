#pragma once
#include "String.h"
#include "pid.h"
#include <Arduino.h>
class sp
{
public:
	String sp_dataString;            // Здесь будут храниться принимаемые данные
	boolean sp_packetAvailable;      // Флаг завершения приема пакета
private:	
	byte id;
	byte instr;
	byte par;
	String sp_startMarker;           // Переменная, содержащая маркер начала пакета
	String sp_stopMarker;            // Переменная, содержащая маркер конца пакета
	int sp_startMarkerStatus;        // Флаг состояния маркера начала пакета
	int sp_stopMarkerStatus;         // Флаг состояния маркера конца пакета
	unsigned int sp_dataLength;               // Флаг состояния принимаемых данных
	
public:
	sp();
	unsigned int hexToDec(String hexString);
	void ParseCommand(pid&);
	void sp_ResetAll();
	void sp_Read();
	
private:
	void sp_Reset();
	void sp_Send(String data);
	
};
