#include <GL/glut.h>	
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "common.h"
#include "line.h"
							
extern void DrawPolygon(int g_iCtrlPntNum, wcPt3D	g_pCtrlPoints[]);

static GLubyte g_iColorRed,g_iColorGreen,g_iColorBlue;
//��Ļ��������
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];

int		g_iCtrlPntNum = 0;	//����ζ������
wcPt3D	g_pCtrlPoints[50];	//����εĶ���

bool g_bPointMoving;	//���¿ո��ѡ��һ�����㣬�ñ�������Ϊtrue
bool g_bFinish;			//����'m'�������������ѡ�񣬸ñ�������Ϊtrue

int iSrcX,iSrcY,iDestX,iDestY; //ֱ����㣬�յ���������

unsigned short g_lineType;
static int tp;

//��ʼ��
void MyInit(void)
{
	g_iCtrlPntNum=0;
	g_bFinish=false;
	g_bPointMoving=false;
	g_lineType=SOLID_LINE;
	tp=0;
}

//��ɫ
void setColour(int r,int g,int b)
{
	g_iColorRed=r;
	g_iColorGreen=g;
	g_iColorBlue=b;
}

//���ػ���
void drawpixel(int i,int j, int color)
{
	static unsigned short t=0x8000;
	static unsigned short t1=t;
	static int nb=8*sizeof(unsigned short);
	static int tx,ty;

	setColour(color>>24,color>>16,color>>8);

	t1=t>>tp;
	tp=(tp+1)%nb;
	if((g_lineType & t1)!=0)
	{
		tx=i;
		ty=j;

		if(0<=ty&&ty<defSCREEN_HEIGHT&&0<=tx&&tx<defSCREEN_WIDTH){
			g_abyScreenImage[ty][tx][0]=g_iColorRed;
			g_abyScreenImage[ty][tx][1]=g_iColorGreen;
			g_abyScreenImage[ty][tx][2]=g_iColorBlue;
		}
	}
}

//��������
void setLineType(unsigned short type)
{
	g_lineType=type;
	tp=0;
}

//��ʼ����Ļ��������Ϊȫ��
void InitScreenImage()
{
	int iHeight,iWidth;
	for(iHeight=0;iHeight<defSCREEN_HEIGHT;iHeight++){
		for(iWidth=0;iWidth<defSCREEN_WIDTH;iWidth++){
			g_abyScreenImage[iHeight][iWidth][0]=255;
			g_abyScreenImage[iHeight][iWidth][1]=255;
			g_abyScreenImage[iHeight][iWidth][2]=255;
		}
	}
}


//�޸�ͼ��
void ModifyScreenImage()
{
	static int i;
	static int dsx,dsy;

	//��ɫʵ��
	setLineType(SOLID_LINE);

	//���ƿ���ͼ
	int line_count;
	if (g_bPointMoving)	//�����ƶ���꣬��Ҫʵʱ���ƶ��ĵ㣬����=������
		line_count = g_iCtrlPntNum;
	else				//��������=������-1
		line_count = g_iCtrlPntNum-1;

	for(i=0;i< line_count;i++){ 
		//����Ϊ���������ά�ռ�����ϵ�еĵ�ת����ƽ������ϵ�ķ����ǣ�
		//���Ե��z���꣬����x��y����ת��Ϊ��ӽ����ǵ�����
		iSrcX=ROUND(g_pCtrlPoints[i].x);
		iSrcY=ROUND(g_pCtrlPoints[i].y);
		iDestX=ROUND(g_pCtrlPoints[i+1].x);
		iDestY=ROUND(g_pCtrlPoints[i+1].y);
		
		DDA(iSrcX,iSrcY,iDestX,iDestY,255<<8 /*��ɫ*/ );
	}

	if (g_bFinish)
	{
		//���ö����ɨ��ת���㷨��ѡ������������ɫ
		//��ͬѧ��ʵ�� DrawPolygon����
		DrawPolygon(g_iCtrlPntNum, g_pCtrlPoints);
	}
}

//��Ӧ���̰���
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	if(iXPos<0||iXPos>=defSCREEN_WIDTH||iYPos<0||iYPos>=defSCREEN_HEIGHT)
		return;

	if(uchKey == 27/*Esc*/) {
		exit(0);
	}
	else if(uchKey==' '){//�ո��ѡ��һ������

		if(g_bFinish){//�����ж��㶼��ѡ���꣬���¿ո��������ѡ�񶥵㼯
			g_iCtrlPntNum=0;
			g_bFinish=false;
		}

		if(g_iCtrlPntNum>=30)//����Ķ��㣬�ᵼ�¼���ֵ��������������������ܱ�ʾ����������
			return;

		//�����µĶ�������
		g_pCtrlPoints[g_iCtrlPntNum].x=iXPos;
		g_pCtrlPoints[g_iCtrlPntNum].y=defSCREEN_HEIGHT-1-iYPos;
		g_pCtrlPoints[g_iCtrlPntNum].z=0;
		g_iCtrlPntNum++;//��������1
		
		g_bPointMoving=false;

	}
	else if(uchKey=='m'||uchKey=='M'){//���������ѡ��

		if(!g_bFinish){
			//���������䣬�������������������ֵ
			g_pCtrlPoints[g_iCtrlPntNum].x = g_pCtrlPoints[0].x;
			g_pCtrlPoints[g_iCtrlPntNum].y = g_pCtrlPoints[0].y;
			g_pCtrlPoints[g_iCtrlPntNum].z = 0;
			g_iCtrlPntNum++;//��������1
			g_bFinish=true;
			g_bPointMoving = false;
		}

	}
	glutPostRedisplay();
}

//��Ӧ���ⰴ��
void Special(int iKey, int iXPos, int iYPos)
{

}

//��Ӧ��갴��
void Mouse(int iButton,int iState,int iXPos, int iYPos)
{

}

//��Ӧ����һ����갴ť�ƶ����
void Motion(int iXPos, int iYPos)
{

}

//��Ӧ�ƶ����
void PassiveMotionFunc(int iXPos, int iYPos)
{
	if(!g_bFinish){//���û����ɶ����ѡ��
		//ʵʱ������λ��
		g_pCtrlPoints[g_iCtrlPntNum].x=iXPos;
		g_pCtrlPoints[g_iCtrlPntNum].y=defSCREEN_HEIGHT-1-iYPos;
		g_pCtrlPoints[g_iCtrlPntNum].z=0;

		if(g_bPointMoving==false){//����ո�ѡ����һ������,��ǰ�Ĺ��λ�û����ܳ�Ϊ�µĶ��㣬��Ӧ����ʾ����Ļ�ϣ���ʵ��"�϶�"��Ч��
			g_bPointMoving=true;
		}
	}
	glutPostRedisplay();

}

void Reshape(int iWidth, int iHeight)
{
	glutReshapeWindow(defSCREEN_WIDTH,defSCREEN_HEIGHT);

}

//��ʼ��
void Init()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	InitScreenImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

//���ƻص�����
void Display() 
{	
	glClear(GL_COLOR_BUFFER_BIT);
	InitScreenImage();
	ModifyScreenImage();
	glDrawPixels(defSCREEN_WIDTH,defSCREEN_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,g_abyScreenImage);
	glutSwapBuffers();
}

int main(int argc,char** argv)
{
	MyInit();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH,defSCREEN_HEIGHT);
	glutCreateWindow("�����ɨ��ת��: �ո��ѡ�����ζ���,'m'���������ѡ��(���㲻�ܳ���30��)");
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutMainLoop();

	return 0;
}

