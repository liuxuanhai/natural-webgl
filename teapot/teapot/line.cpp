#include <math.h>
#include "common.h"
#include "line.h"

extern void drawpixel(int i,int j, int color);

//DDA�㷨����ֱ��
//(x0,y0),�����������
//(x1,y1),�յ���������
//setPixel,���ػ��ƺ�������main.cpp�ж��壩
void DDA(int x0,int y0,int x1,int y1,int color)
{
	//�㷨ʵ�ִ���
	int x,temp;
	      float dx, dy, y, k;
	      dx=x1-x0;
          dy=y1-y0;
	      k=dy/dx; 
          y = y0;
	      for (x=x0; x<=x1; x++)
          {
            drawpixel(x, (int)(y+0.5),color);	
	        y=y+k;
          }
}

//Bresenham�㷨����ֱ��
//����ͬDDA
void Bresenham(int x0,int y0,int x1,int y1,int color)
{
	//�㷨ʵ�ִ���
}

//�е��߻�������ֱ��
//����ͬDDA
void MidPoint(int x0,int y0,int x1,int y1,int color)
{
	//�㷨ʵ�ִ���

}