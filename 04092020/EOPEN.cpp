#include "EOPEN.h"


CANVAS* OPENCLSET(string title, int w, int h, int cw, int ch, char OS, const char TYPE)
{
	SETCONSOLESIZE(title, w, h, OS, TYPE);
	if (cw == -1)
		cw = w;
	if (cw == -1)
		ch = h;
	return SETCANVASSIZE(w, h);
}
void SETCONSOLESIZE(string title, int w, int h, char OS, const char TYPE)
{
	switch (OS)
	{
	case WINDOW:
	{
		HWND console = GetConsoleWindow();
		RECT r;
		string tt = "Title : " + title;
		system(tt.c_str());
		GetWindowRect(console, &r);
		MoveWindow(console, 50, 50, 640, 480, SWP_SHOWWINDOW | SWP_NOMOVE);

		if (TYPE == BORDERLESS)
		{
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO info;
			GetConsoleScreenBufferInfo(handle, &info);
			COORD new_size =
			{
				info.srWindow.Right - info.srWindow.Left + 1,
				info.srWindow.Bottom - info.srWindow.Top + 1
			};
			SetConsoleScreenBufferSize(handle, new_size);

			ReleaseDC(console, NULL);
		}
		break;
	}
	default:
		break;
	}
}
CANVAS* SETCANVASSIZE(int w, int h)
{
	CANVAS* canvas = (CANVAS*)malloc(sizeof(CANVAS));
	CANVAS temp(w, h);
	memcpy(canvas, &temp, sizeof(CANVAS));
	canvas->pixels = (unsigned int*)malloc(sizeof(int) * canvas->w * canvas->h);
	memcpy(canvas->pixels, temp.pixels, sizeof(int) * canvas->w * canvas->h);
	return canvas;
}

EMATRIXF* SETCAMERA(float fx, float fy, float cx, float cy, float rx, float ry, float rz, float tx, float ty, float tz)
{
	float camera_Matrix[] =
	{
		fx,0,cx, cos(rx) * cos(ry), cos(rx) * sin(ry) * sin(rz) - sin(rx) * cos(rz), cos(rx) * sin(ry) * cos(rz) + sin(rx) * sin(rz),tx,
		0,fy,cy, sin(rx) * cos(ry), sin(rx) * sin(ry) * sin(rz) + cos(rx) * cos(rz), sin(rx) * sin(ry) * cos(rz) - cos(rx) * sin(rz),ty,
		0, 0, 1, -sin(ry), cos(ry) * sin(rz), cos(ry) * cos(rz),tz
	};
	EMATRIXF* CAMERA_ptr = (EMATRIXF*)malloc(sizeof(EMATRIXF));
	CAMERA_ptr->m = 3;
	CAMERA_ptr->n = 7;
	CAMERA_ptr->vectors = (float*)malloc(sizeof(float) * 21);
	memmove(CAMERA_ptr->vectors, camera_Matrix, sizeof(float) * 21);
	return CAMERA_ptr;
}
EMATRIXF* ESTL2EMATRIX(ESTL estl)
{
	unsigned long number = estl.num * 18;
	EMATRIXF* ES = (EMATRIXF*)malloc(sizeof(EMATRIXF));
	ES->m = estl.num * 3 * 2;
	ES->n = 3;
	ES->vectors = (float*)malloc(sizeof(float) * number);
	int k = 0;
	for (unsigned long i = 0; i < number; i += 18)
	{
		//-----------------------------------------------
		//t1
		ES->vectors[i] = estl.vertices[k].m_x;
		ES->vectors[i + 1] = estl.vertices[k].m_y;
		ES->vectors[i + 2] = estl.vertices[k].m_z;
		//t2
		ES->vectors[i + 3] = estl.vertices[k + 1].m_x;
		ES->vectors[i + 4] = estl.vertices[k + 1].m_y;
		ES->vectors[i + 5] = estl.vertices[k + 1].m_z;
		//-----------------------------------------------
		//t2
		ES->vectors[i + 6] = estl.vertices[k + 1].m_x;
		ES->vectors[i + 7] = estl.vertices[k + 1].m_y;
		ES->vectors[i + 8] = estl.vertices[k + 1].m_z;
		//t3
		ES->vectors[i + 9] = estl.vertices[k + 2].m_x;
		ES->vectors[i + 10] = estl.vertices[k + 2].m_y;
		ES->vectors[i + 11] = estl.vertices[k + 2].m_z;
		//-----------------------------------------------
		//t3
		ES->vectors[i + 12] = estl.vertices[k + 2].m_x;
		ES->vectors[i + 13] = estl.vertices[k + 2].m_y;
		ES->vectors[i + 14] = estl.vertices[k + 2].m_z;
		//t1
		ES->vectors[i + 15] = estl.vertices[k].m_x;
		ES->vectors[i + 16] = estl.vertices[k].m_y;
		ES->vectors[i + 17] = estl.vertices[k].m_z;
		k+=3;
	}
	return ES;
}

void OPENCL_BMP(CANVAS* canvas, string file_name, int x, int y, int pixel)
{
	elog << "OPEN BMP FILE : " + file_name + "\n";
	EBMP ebmp(file_name);
	if (&ebmp != NULL)
		canvas->PushEBMP(&ebmp, x, y, pixel);
}
void GRAPHICDISPLAYER(CANVAS& canvas)
{
	canvas.c2bit24("./CANVAS.bmp");
	DrawBitBlt(0, 0, "./CANVAS.BMP");
}
void GRAPHICDISCLEAR(CANVAS& canvas)
{
	canvas.CLEARCANVAS();
}









