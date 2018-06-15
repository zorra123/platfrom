#include "sp.h"
#include "pid.h"
//0xFF 0xFF 0x03 	0x03 	0x01 			0x1E 	0xFA 0xFA
//0xFF 0xFF len(n+2) id 	instruction 	par 	0xFA 0xFA
sp::sp()
{
	sp_startMarker = "0xFF 0xFF";     // Так будет выглядеть маркер начала пакета
	sp_stopMarker = " 0xFA 0xFA";      // Так будет выглядеть маркер конца пакета
	sp_dataString.reserve(64);     // Резервируем место под прием строки данных
	sp_ResetAll();                 // Полный сброс протокола
}
void sp::sp_ResetAll()
{
	sp_dataString = "";           // Обнуляем буфер приема данных
	sp_Reset();                   // Частичный сброс протокола
}
void sp::sp_Reset()
{
	sp_startMarkerStatus = 0;     // Сброс флага маркера начала пакета
	sp_stopMarkerStatus = 0;      // Сброс флага маркера конца пакета
	sp_dataLength = 0;            // Сброс флага принимаемых данных
	sp_packetAvailable = false;   // Сброс флага завершения приема пакета
}
void sp::sp_Read()
{
	while(Serial.available() && !sp_packetAvailable)            // Пока в буфере есть что читать и пакет не является принятым
	{
		char bufferChar = (char)Serial.read();                           // Читаем очередной байт из буфера
		if(sp_startMarkerStatus < sp_startMarker.length())        // Если стартовый маркер не сформирован (его длинна меньше той, которая должна быть) 
		{  
			 if(sp_startMarker[sp_startMarkerStatus] == bufferChar)   // Если очередной байт из буфера совпадает с очередным байтом в маркере
				 {
				   sp_startMarkerStatus++;                                // Увеличиваем счетчик совпавших байт маркера
				 }
			 else
				 {
				   sp_ResetAll();                                         // Если байты не совпали, то это не маркер. Нас нае****, расходимся. 
				 }
		}  
		else
		{
		 // Стартовый маркер прочитан полностью
			if(sp_dataLength <= 0)                                 // Если длинна пакета на установлена
			{	
				if (bufferChar!=' '){
				String len = "";
				len+=bufferChar;
				for(int i =0;i<3;)
				{
					if(Serial.available()){
						len += (char)Serial.read();
						i++;
					}
				}
				sp_dataLength = hexToDec(len)*4+hexToDec(len);                          // Значит этот байт содержит длину пакета данных
				}
			}
			else                                                    // Если прочитанная из буфера длинна пакета больше нуля
			{
				if(sp_dataLength > sp_dataString.length())            // Если длинна пакета данных меньше той, которая должна быть
				{
					sp_dataString += bufferChar;                  // прибавляем полученный байт к строке пакета
					//Serial.println(sp_dataString);
				}
				else                                                  // Если с длинной пакета данных все нормально
				{
					if(sp_stopMarkerStatus < sp_stopMarker.length())    // Если принятая длинна маркера конца пакета меньше фактической
					{
						if(sp_stopMarker[sp_stopMarkerStatus] == bufferChar)  // Если очередной байт из буфера совпадает с очередным байтом маркера
						{
							sp_stopMarkerStatus++;                              // Увеличиваем счетчик удачно найденных байт маркера
							if(sp_stopMarkerStatus == sp_stopMarker.length())
							{
								// Если после прочтения очередного байта маркера, длинна маркера совпала, то сбрасываем все флаги (готовимся к приему нового пакета)
								//sp_Reset();    
								sp_packetAvailable = true;                        // и устанавливаем флаг готовности пакета
							}
					}
					else
					{
						sp_ResetAll();                                      // Иначе это не маркер, а х.з. что. Полный ресет.
					}
					}
				  //
				}
			} 
		}    
	}
}
void sp::ParseCommand(pid& pid){
	//Serial.println("1");
	String sp_dataString_clear = "";  
	String tmp = "";
	for(int i =0;i<sp_dataString.length();i++){
		if(sp_dataString[i]!=' ')
			sp_dataString_clear+=sp_dataString[i];
	}
	for(int i =0;i<sp_dataString_clear.length()/4;i++)
	{
		for(int j =i*4;j<i*4+4;j++)
		{
			tmp += sp_dataString_clear[j];
		}
		switch(i){
			case 0:
				id = hexToDec(tmp);
				//Serial.print("id ");
				//Serial.println(id);
				break;
			case 1:
				instr = hexToDec(tmp);
				//Serial.print("instr ");
				//Serial.println(instr);
				break;
			case 2:
				par = hexToDec(tmp);
				//Serial.print("par ");
				//Serial.println(par);
				break;
		}
		//Serial.println(tmp);
		tmp="";
	}
	if(instr==3) par =0;
	pid.initialization(id,instr,par);

}
unsigned int sp::hexToDec(String hexString) {
	int i;
	if (hexString[0]=='0'&&hexString[1]=='x') i = 2;
	else i =0;
	unsigned int decValue = 0;
	int nextInt;
  
	for (; i < hexString.length(); i++) {

		nextInt = int(hexString.charAt(i));
		if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
		if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
		if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
		nextInt = constrain(nextInt, 0, 15);

		decValue = (decValue * 16) + nextInt;
	}
  
	return decValue;
}
String sp::decToHex(byte decValue) {
  
  String hexString = String(decValue, HEX);
  hexString.toUpperCase();
  if(hexString.length()==2) hexString = "0x" + hexString;
  else hexString = "0x0" + hexString;
  return hexString;
}

void sp::Send(byte *data){
	String send = sp_startMarker+" "+decToHex(data[0])+" "+decToHex(data[1])+sp_stopMarker;
	//String send = sp_startMarker+data+sp_stopMarker;
	Serial.println(send);
}