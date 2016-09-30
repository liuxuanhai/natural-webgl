#ifndef COMMON_H
#define COMMON_H

#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700	

/*��������*/
#define SOLID_LINE	0xffff//ʵ��
#define DASH_LINE	0xf8f8//����
#define DOT_LINE	0x8888//����

#define ROUND(a) ((int)(a+0.5))

typedef struct{
	float x,y,z;
}wcPt3D;

/*��������*/
inline void swap(int& ia,int& ib)
{
	ia=ia^ib;
	ib=ib^ia;
	ia=ia^ib;
}

#endif