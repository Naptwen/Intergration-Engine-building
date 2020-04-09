#include "ESTL.h"

vertex_double::vertex_double(char* face)
{
	char f1[4] = { face[0],face[1],face[2],face[3] };
	char f2[4] = { face[4],face[5],face[6],face[7] };
	char f3[4] = { face[8],face[9],face[10],face[11] };
	float xx = *((float*)f1);
	float yy = *((float*)f2);
	float zz = *((float*)f3);
	this->m_x = double(xx);
	this->m_y = double(yy);
	this->m_z = double(zz);
}


ESTL::ESTL(string file_name)
{
	this->num = 0;
	this->vertices = nullptr;
	ifstream stl;
	stl.open(file_name, ios::in | ios::binary);
	if (stl.good())
	{
		//READING HEADER
		stl.seekg(0, ios::beg);
		char* data = (char*)calloc(80, sizeof(char));
		stl.read((char*)data, 80);
		//THE NUMBER OF TRIANGLE
		unsigned long num_tri_long;
		char* num_tri_char = (char*)calloc(4, sizeof(char));		
		stl.read(num_tri_char, 4);
		num_tri_long = *((unsigned long*)num_tri_char);
		this->num = (unsigned long)num_tri_long;


		this->vertices = (vertex_double*)malloc(sizeof(vertex_double) * 3 * num_tri_long);
		for (int i = 0; i < num_tri_long  * 3; i+=3)
		{
			char* face = (char*)malloc(50 * sizeof(char));
			stl.read(face, 50);
			vertex_double p1(face + 12);
			vertex_double p2(face + 24);
			vertex_double p3(face + 36);
			memcpy(&vertices[i],&p1,sizeof(vertex_double));
			memcpy(&vertices[i + 1], &p2, sizeof(vertex_double));
			memcpy(&vertices[i + 2], &p3, sizeof(vertex_double));
			free(face);
		}

		free(num_tri_char);
		free(data);
		stl.close();
	}
	else
	{
		printf(" failed to read from file\n");
	}
}
void ESTL::info()
{
	elog <<"********************************************************\n";
	_logtxt<<" SIZE : " +  to_string(this->num) + "\n";
	_logtxt <<" #vertices : " + to_string(this->num * 3) + "\n";
	for (int i = 0; i < this->num * 3; i+= 3)
	{
		_logtxt << 
			"t1 [" + to_string((float)(this->vertices[i]).m_x) + "]"
			+ " [" + to_string((float)(this->vertices[i]).m_y) + "]"
			+ " [" + to_string((float)(this->vertices[i]).m_z) + "]"
		+" t2 [" + to_string((float)(this->vertices[i + 1]).m_x) + "]"
			+ " [" + to_string((float)(this->vertices[i + 1]).m_y) + "]"
			+ " [" + to_string((float)(this->vertices[i + 1]).m_z) + "]"
		+" t3 [" + to_string((float)(this->vertices[i + 2]).m_x) + "]"
			+ " [" + to_string((float)(this->vertices[i + 2]).m_y) + "]"
			+ " [" + to_string((float)(this->vertices[i + 2]).m_z) + "]\n";
	}
	_logtxt <<"********************************************************\n";
}