
#ifndef LINE_H
#define LINE_H


//DDA�㷨����ֱ��
//(x0,y0),�����������
//(x1,y1),�յ���������
//setPixel,���ش�����
void DDA(int x0,int y0,int x1,int y1,int color);

//Bresenham�㷨����ֱ��
//����ͬDDA
void Bresenham(int x0,int y0,int x1,int y1,int color);

//�е��߻�������ֱ��
//����ͬDDA
void MidPoint(int x0,int y0,int x1,int y1,int color);

//��ɫ��ֵ������졢�̡���3������Ϊ��ΪR��G��B����ô��color=(R<<24+G<<16+B<<8)

void drawpixel(int i,int j, int color);

#endif