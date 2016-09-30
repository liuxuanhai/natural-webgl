#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "line.h"

#define defSCREEN_WIDTH 800								
#define defSCREEN_HEIGHT 600

int g_lineNum;
bool g_startSelect,g_endSelect,g_finish;
wcPt2D *g_lines;


static GLubyte g_iColorRed,g_iColorGreen,g_iColorBlue;
//��Ļ��������
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];


unsigned short g_lineType;
static int tp;
//�������ض��ĳ�ʼ��������ڴ˺����У�
void MyInit(void)
{
	g_lineNum = 0;
	g_startSelect = g_endSelect = g_finish = false;
	//��̬������ֱ�߶˵���ڴ�ռ�
	g_lines=(wcPt2D*)malloc(100*sizeof(wcPt2D));
	g_lineType=SOLID_LINE;
	tp=0;
}

//��ɫ���÷���
void setColour(int r,int g,int b)
{
	g_iColorRed = r;
	g_iColorGreen = g;
	g_iColorBlue = b;
}

//���ػ���
void drawpixel(int i,int j, int color)
{
	static unsigned short t=0x8000; //�ȼ۵Ķ����Ʊ�ʾ��1000 0000 0000 0000
	static unsigned short t1=t;
	static int nb = 8*sizeof(unsigned short); //t��bit��
	static int tx,ty;

    //ע��:���ڴ���RGB32�ĸ�����������˳��Ϊ��BGRA(���8bit��Alphaͨ��)
	setColour(color>>8,color>>16,color>>24);

	t1 = t>>tp; //����tpλ
	tp=(tp+1) % nb;
	if((g_lineType & t1) != 0)  //���Ϳ���
	{
		tx=i;
		ty=j;
		if( 0<=ty && ty<defSCREEN_HEIGHT && 0<=tx && tx<defSCREEN_WIDTH){
			g_abyScreenImage[ty][tx][0]=g_iColorRed;
			g_abyScreenImage[ty][tx][1]=g_iColorGreen;
			g_abyScreenImage[ty][tx][2]=g_iColorBlue;
		}
	}
}

//������������
void setLineType(unsigned short type)
{
	g_lineType = type;
	tp=0;
}

//��ʼ����Ļ��������Ϊȫ��
void InitScreenImage()
{
	int iHeight,iWidth;
	for(iHeight=0;iHeight<defSCREEN_HEIGHT;iHeight++){
		for(iWidth=0;iWidth<defSCREEN_WIDTH;iWidth++){
			g_abyScreenImage[iHeight][iWidth][0]=0;
			g_abyScreenImage[iHeight][iWidth][1]=0;
			g_abyScreenImage[iHeight][iWidth][2]=0;
		}
	}
}

//�޸�ͼ��
void ModifyScreenImage()
{
	static int i;
	int color;
	setColour(255,255,0);   //��ɫ
	setLineType(SOLID_LINE);  //ʵ��
	color = (g_iColorRed<<8)+(g_iColorGreen<<16)+(g_iColorBlue<<24);
	for(i=0;i<g_lineNum;i++){
		//������Ӧ�Ļ����㷨
		DDA(ROUND(g_lines[i*2].x),
			ROUND(g_lines[i*2].y),
			ROUND(g_lines[i*2+1].x),
			ROUND(g_lines[i*2+1].y),
			color);
	}
}

//��Ӧ���̰����Ļص�����
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	if(iXPos<0 || iXPos>=defSCREEN_WIDTH || iYPos<0 || iYPos>=defSCREEN_HEIGHT)
		return;

	if(uchKey == 27/*Esc*/) {
		exit(0);
	}
	else 
	if(uchKey==' ')/*����ѡ��ֱ�߶ε������˵�*/{
		if(!g_startSelect/*ֱ�߶���ʼ�˵���δѡ��*/){

		    if(g_lineNum>=40) //ֱ�߶���Ŀ����40
			{
				g_finish=true;
				return;
			}
			//����ǰλ����Ϊ��g_lineNum��ֱ�߶ε�������꣺
			g_lines[g_lineNum*2].x = iXPos;
			g_lines[g_lineNum*2].y = defSCREEN_HEIGHT-iYPos;
			g_lineNum++; //ֱ�߶������� 1
			g_startSelect=true; 
		}
		else{ //g_startSelect == true��
			//����ǰλ����Ϊ��g_lineNum��ֱ�߶ε��յ����꣺
			g_lines[g_lineNum*2-1].x = iXPos;
			g_lines[g_lineNum*2-1].y = defSCREEN_HEIGHT-iYPos;
			//g_lineNum++; //ֱ�߶������� 1
			g_startSelect = false;
			g_endSelect = true;
			glutPostRedisplay(); //��ǵ�ǰ������glutMainLoop��һ��ѭ��ʱ��Ҫ���»��ơ�
		}
	}
	else if(uchKey=='c'|| uchKey=='C'){//����
		g_lineNum=0;
		g_startSelect = g_endSelect = g_finish = false;
		glutPostRedisplay();
	}
}

//��Ӧ���ⰴ���Ļص�����
void Special(int iKey, int iXPos, int iYPos)
{

}

//��Ӧ��갴���Ļص�����
void Mouse(int iButton,int iState,int iXPos, int iYPos)
{

}

//��Ӧ����һ����갴ť�ƶ����Ļص�����
void Motion(int iXPos, int iYPos)
{

}

//��Ӧ�ƶ����Ļص�����
void PassiveMotionFunc(int iXPos, int iYPos)
{
	if(!g_finish){//�����û����ɿ��Ƶ��ѡ��
		if(g_startSelect) //ֱ�߶ε�����Ѿ�ѡ��
		{   //�����ĵ�ǰλ����Ϊֱ�߶ε���ʱ�յ㣺
			g_lines[g_lineNum*2-1].x = iXPos;
			g_lines[g_lineNum*2-1].y = defSCREEN_HEIGHT-iYPos;
			//��������ƶ�ʱ�ļ���ֱ�߶Σ���Ϊ����Ƥ�������
			glutPostRedisplay(); //��ǵ�ǰ������glutMainLoop��һ��ѭ��ʱ��Ҫ���»��ơ�
		}
	}
}

//�ı䵱ǰ���ڵĴ�Сʱִ�У�
void Reshape(int iWidth, int iHeight)
{
	glutReshapeWindow(defSCREEN_WIDTH,defSCREEN_HEIGHT);  //���ֲ���
	//glutReshapeWindow(iWidth,iHeight);  
}

//ֻ��Ҫ����ʼʱִ��1�εĳ�ʼ�����붼Ӧ�÷��ڴ˷����У�
//�����ñ���ɫ������ϵͳ��
void Init()  
{
	glClearColor(0.0,0.0,0.0,0.0); //���RGBAģʽ�µ���ɫ������
	glShadeModel(GL_FLAT);  //������ɫģʽ
	InitScreenImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT,1); //�������صĴ洢ģʽ����ѹ�������룩
}

//��Ҫ���ų�������У����£���Ⱦ�Ĳ���Ӧ�÷��ڴ˻ص������У�
void Display() 
{	
	glClear(GL_COLOR_BUFFER_BIT); //���ָ����GL_COLOR_BUFFER_BIT������
	InitScreenImage();
	ModifyScreenImage();
	glDrawPixels(defSCREEN_WIDTH,
		         defSCREEN_HEIGHT,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 g_abyScreenImage);
	glutSwapBuffers(); //����˫���������������ָ��
}

//�����������
int main(int argc,char** argv)
{
	printf("%s","ʵ��1��ֱ�ߵĻ���Demoʹ��˵����\n");
	printf("%s","1.�ո����ѡ��ֱ�߶εĶ˵�\n");
	printf("%s","2.����϶�����һ���˵��Ѿ�ȷ��ʱ����̬�������ĵ�ǰλ��Ϊ��һ���˵㻭ֱ��\n");
	printf("%s","3.C��������\n");
	printf("%s","4.Esc�����˳�ϵͳ\n");
	MyInit(); //�������ض��ĳ�ʼ��
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //��ʼ��Ϊ˫���塢RGBAɫ��ģʽ
	glutInitWindowSize(defSCREEN_WIDTH,defSCREEN_HEIGHT);
	glutCreateWindow("ʵ��һ��ֱ�߶�ɨ��ת���㷨��ʵ��"); //������ͼ����
	Init();  //����ʼʱ��һ���Գ�ʼ��
	//ע��ص�������
	glutDisplayFunc(Display); //��������ʱ���õĻص�����
	glutReshapeFunc(Reshape); //���ڵ�����Сʱ���õĻص�����
	glutKeyboardFunc(Keyboard); //���̰���ʱ���õĻص�����
	glutSpecialFunc(Special); //���ⰴ���Ļص�����
	glutMouseFunc(Mouse);   //����갴ť����ʱ���õĻص�����
	glutMotionFunc(Motion); //������ƶ�ʱ���õĻص�����
	glutPassiveMotionFunc(PassiveMotionFunc);  //����갴�°�ť���϶�ʱ���õĻص�����
	//ע����ϣ�����������ѭ����
	glutMainLoop();  
	return 0;
}
