#include <math.h>
#include "common.h"
#include "line.h"

extern void drawpixel(int i,int j, int color);

bool re_arrange_xy(int &x0, int &y0, int &x1, int &y1, float &dx, float &dy) {
	dx = x1-x0;
	dy = y1-y0;
	bool x_y_swap = false;
	if(fabs(dx) < fabs(dy)){  //����k�����ڵ����
		swap(x0,y0);
		swap(x1,y1);
		swap(dx,dy);
		x_y_swap = true;  //true��ʾ����ϵ�任����
	}
	if(x0>x1){
		swap(x0,x1);
		swap(y0,y1); //�˵��������Ե�����
		dx = -dx;
		dy = -dy;    //dx��֤Ϊ��������k���ܸı�
	}

	return x_y_swap;
}

//DDA�㷨����ֱ��
//(x0,y0),�����������
//(x1,y1),�յ���������
//setPixel,���ػ��ƺ�������main.cpp�ж��壩
void DDA(int x0,int y0,int x1,int y1,int color)
{
	//�㷨ʵ�ִ���
	int x;
	float dx, dy, y, k; //yҪ������������
	bool x_y_swap = re_arrange_xy(x0, y0, x1, y1, dx, dy);
	k = dy/dx;
	for(x = x0,y=y0; x<=x1; x++)
    {
        if(!x_y_swap)   drawpixel(x,(int)(y+0.5),color);
		else            drawpixel((int)(y+0.5),x,color); 
		            //��y-x����ϵ������k�����ڵ�����Ҳ�����
		y = y+k;   //k�Ѿ����۹���
    }
}

//Bresenham�㷨����ֱ��
//����ͬDDA
void Bresenham(int x0,int y0,int x1,int y1,int color) 
{
	int x, y, dx, dy, i,e;
	int flag1=1,flag2=1; //û���õ�float
	bool changeFlag = false;
	dx=abs(x0-x1); //ȡ����ֵ�������ж����i
	dy=abs(y0-y1);
	if(x0>x1) {
		flag1 = -1;
	}
	if(y0>y1) {
		flag2 = -1;
	}
	if(dx<dy) { // |k|>1����k�����������dx���Ե���0
		swap(dx,dy);
		changeFlag = true;
	}
	x=x0; y=y0; //ָ�����ߵ����
	e = -dx;
	for(i=0; i<=dx; i++) {
		drawpixel(x,y,color);
		if(!changeFlag) x += flag1; //�˵����⵼�µķ�������
		else            y += flag2; //kֵ���⻻������ϵ 
		e = e+2*dy;
		if(e>=0) { //e=0��Լ�� 
		    if(!changeFlag) y += flag2;
		    else            x += flag1; 
			e = e-2*dx;
		}
	}
} 

/*
void Bresenham(int x0,int y0,int x1,int y1,int color)
{
    //�㷨ʵ�ִ���
	int x,y;  //�����y������������
	float dx,dy,e;  
	bool x_y_swap = re_arrange_xy(x0, y0, x1, y1, dx, dy);
	e = -dx;
	int sign = 1;   //��������ʹ����һ�����
	if(dy<0) sign=-1;  //k<0�����
	for(x=x0, y=y0; x<=x1; x++) {
		if(x_y_swap)  drawpixel(y, x, color);
		else          drawpixel(x, y, color);
		e = e+2*sign*dy; //k<0ʱ
		if(e>=0) {
			y += sign;  //y����ı仯
			e = e-2*dx;
		}
	}
}
*/


//�е��߻�������ֱ��
//����ͬDDA
void MidPoint(int x0,int y0,int x1,int y1,int color)
{
    //�㷨ʵ�ִ���
    int a, b, d1, d2, d, x, y;
    a=y0-y1; b=x1-x0; 
    x=x0; y=y0; 
	int flag1=(b >= 0 ? 1 : (b = -b, -1)); //x-y����ϵ����y�����й� 
    int flag2=(a <= 0 ? 1 : (a = -a, -1)); //x-y����ϵ����x�����й�
	                       //ʹa������b�Ǹ�����kΪ��
	d2=2* (a+b);  //�Գ��ԣ�������ȷ����
    drawpixel(x, y, color); //����ʼ��
	if(-a <= b) {
        d=2*a+b;
        d1=2*a; 
        while (x!=x1)
        {
            if (d<0) { 
				y += flag2; 
				d+=d2; 
			}
            else { 
				d+=d1;
			}
			x += flag1;
            drawpixel (x, y, color);
        } 
	}

	else { //����|k|>1����k�����
		d=2*b+a;
        d1=2*b;  //��ת����
        while (y!=y1)
        {
            if (d<0) { 
				d+=d1; 
			}
            else { 
				x += flag1; 
				d+=d2;
			}
			y += flag2; //flag2�Ǹ�y��
            drawpixel (x, y, color);
        }
	}
}

