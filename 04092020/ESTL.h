#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "ELOG.h"
using namespace std;
//CODE STRUCTURE FROM http://www.sgh1.net/posts/read-stl-file.md
struct vertex_double {
public:
	double m_x;
	double m_y;
	double m_z;
	vertex_double::vertex_double(char* face);
};

struct ESTL
{
public:
	vertex_double* vertices;
	unsigned long num;
	ESTL::ESTL(string file_name);
	void ESTL::info();
};