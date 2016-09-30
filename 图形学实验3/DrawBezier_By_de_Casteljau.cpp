#include <GL/glut.h>	
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "line.h"

void DrawBezier_By_de_Casteljau(int g_iCtrlPntNum, wcPt3D	g_pCtrlPoints[])
{
		/*ansX��ansY���������Ƿֱ������洢g_pCtrlPoints������x�����ֵ��y�����ֵ
		����ansX�����ansY����Ҳ��Ҫ�����洢���ƹ����е�ֵ��*/
		double ansX[1000],ansY[1000];

		/*X�����Y�������������洢ÿһ�β�����֮���ֵ*/
		double X[1000],Y[1000];

		/*pitch����ָ����ÿһ�ݵĳ���*/
		double pitch = 1.0 / (g_iCtrlPntNum * 30);

		/*now����ǰ�õĳ���*/
		double now = 0;

		/*ans��һ����������������¼ÿһ�β������ֵ���±꣬��rest��������¼һ�β����л��ж��ٸ���ʣ��*/
		int ans = 0,rest = g_iCtrlPntNum;

		/*�Ȱ���������ȫ����ֵ����*/
		for(int i = 0; i < g_iCtrlPntNum; ++i)
		{
			ansX[i] = g_pCtrlPoints[i].x;
			ansY[i] = g_pCtrlPoints[i].y;
		}

		/*��ΪDDA�㷨������Ҫ�����㣬���������һ���������ʼ�㣬�ȴ洢��ȥ*/
		X[ans] = ansX[0];
		Y[ans] = ansY[0];

		/*�����������п��ܵ�now��ÿ����һ�Σ�������++��now��ֵ�����仯���������½�ԭg_pCtrlPoints
		�����е��������ֵ����������Ȼ��rest��Ҫ������Ϊg_iCtrlPntNum-1.
		Ȼ�����rest��=1ʱ��һֱ�������Ƶ�������ǵ����㣬ֱ��������ս����Ȼ��������ս���ӽ�X
		��Y�����У���DDA�㷨����ǰ���ǰһ��������������*/
		while(now <= 1 - pitch)
		{
			ans ++;
			now += pitch;
			rest = g_iCtrlPntNum - 1;
		
			for(int i = 0; i < g_iCtrlPntNum; ++i)
			{
				ansX[i] = g_pCtrlPoints[i].x;
				ansY[i] = g_pCtrlPoints[i].y;
			}

			while(rest)
			{
				/*����������ǵ��㷨�����ϵ��ƻ�ȥ�����Һ�����������Ƶ���ÿ����һ�㣬��һ��ĸ�����
				����1����*/
				for(int i = 0; i < rest; ++i)
				{
					ansX[i] = (1 - now) * ansX[i] + now * ansX[i + 1];
					ansY[i] = (1 - now) * ansY[i] + now * ansY[i + 1];
				}
				rest --;
			}
			 /*����ǰ����洢��ȥ*/
			X[ans] = ansX[0];
			Y[ans] = ansY[0];
			DDA((int)X[ans - 1],(int)Y[ans - 1],(int)X[ans],(int)Y[ans],(255 << 24));
		}
}
   