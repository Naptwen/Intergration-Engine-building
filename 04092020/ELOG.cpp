#pragma once
#include "ELOG.h"

void LOG::Clearlog()
{
	std::ofstream log;
	log.open(this->file_name, std::ofstream::out | std::ofstream::trunc);
	log.close();
}
void LOG::operator << (string A)
{
	this->Filelog(A);
}
void LOG::Filelog(string txt, ...)
{
	ofstream log;
	log.open("LOG.txt", std::ios_base::app);
	log << "[time]" << currentDateTime() << " : [TEXT] : ";
	log << txt;
	log.close();

}


const std::string currentDateTime() {
	time_t     now = time(0); //���� �ð��� time_t Ÿ������ ����
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ ��Ʈ��

	return buf;
}

