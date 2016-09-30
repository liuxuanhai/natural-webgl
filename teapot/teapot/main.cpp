#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glaux.h>
#include <math.h>
#include <windows.h>

void setLightPos(int i)   //�趨��Դ��λ��
{
  GLfloat lightPos[5][4]={ {  0.0,  0.0, 10.0, 1.0},     //��ǰ��
                           {  0.0,  10.0, 0.0, 1.0},     //���Ϸ�
				           {  0.0, -10.0, 0.0, 1.0},     //���·�
                           {-10.0,  10.0, 0.0, 1.0},     //���Ϸ�
                           { 10.0, -10.0, 0.0, 1.0}      //���·�
                         };
   glLoadIdentity();
   //ָ����ԴGL_LIGHT0��λ��ΪlightPos[i]��
   glLightfv(GL_LIGHT0,GL_POSITION,lightPos[i]);
}

void setLight(int lightNo)
{
  //����һ�����еİ�ɫ��Դ��
  GLfloat ambientLight[]   =  {0.3,0.3,0.3,1.0};     //����������
  GLfloat diffuseLight[]      =  {0.5,0.5,0.5,1.0};  //���������
  GLfloat specularLight[]   =  {0.5,0.5,0.5,1.0};    //���淴���
  
  glEnable(GL_LIGHTING);  //���ù���ģ��
  setLightPos(lightNo);   //����lightNo���ù�Դλ��
  //���������ù�ԴLIGHT0��
  glEnable(GL_LIGHT0);  //����GL_LIGHT0
  glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);   //���û�����
  glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);       //���������
  glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight); //���þ��淴���
  
  glMaterialf(GL_FRONT, GL_SHININESS, 5.0);          //���ϵľ��淴������

  glEnable(GL_COLOR_MATERIAL);   //������ɫ���ٷ���ʵ���ʲ�����
  //���ö��������Ļ���������������
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  
  //���ò��ʵ�����,�μ�p.190��5.4��p.193��5.5��
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight);   //�������ԵĻ����� ��p.193)
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseLight);       //�������Ե��������
  glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight); //�������Եľ��淴���
}


//װ������-------------------------
/* ����LoadGLTextures  
 * ��ȡһ����BMPImageFileNameָ����BMP�ļ���Ϊ����
 * �ر����ѣ�
 * ��1�����ļ��ĳ��Ϳ������2�ĳ���
 * ��2�����ļ���ͼ��洢��ʽ������24bit��BMP�ļ�
 * ���ʧ�ܣ�����0������ɹ�������������  
 */   

GLuint LoadGLTextures(char* BMPImageFileName)   
{
    GLuint  texture_ID=0;                                  // ����ID
    AUX_RGBImageRec *TextureImage[1];     // ��������Ĵ洢�ռ䣨�������������ʽ��
    memset(TextureImage,0,sizeof(void *)*1);   // ��ָ����Ϊ NULL
    // ��������λͼ
    if (TextureImage[0]=auxDIBImageLoadA(BMPImageFileName))
    {
       glGenTextures(1,                      //  1.����1�������ʶ��
		                         &texture_ID    //  �洢��texture_ID��
			                    ); 
       //2.������ID�������������GL_TEXTURE_2D�󶨣� 
       glBindTexture(GL_TEXTURE_2D, texture_ID);
       //3.������λͼ����������������
       glTexImage2D(
		       GL_TEXTURE_2D, 
	           0, //ָ��mipmap�ĵȼ���0��Ӳ���Զ����� 
			   3, //ͼ���ڲ���ʽ��1���Ҷ�ͼ��, 3��RGB, 4��RGBA
			   TextureImage[0]->sizeX, //ͼ���ȣ�2�ĳ��ݣ�
			   TextureImage[0]->sizeY, //ͼ��߶ȣ�2�ĳ��ݣ�
			   0,       //����߿���
			   GL_RGB,  //����Դ��ʽ
			   GL_UNSIGNED_BYTE,      //����Դ����
			   TextureImage[0]->data  //ԭʼͼ������
			   );
	   //4.�����������״̬��
       glTexParameteri(
		               GL_TEXTURE_2D,
		               GL_TEXTURE_MIN_FILTER, //������Сʱ���˲�����
					   GL_LINEAR); // �����˲�
       glTexParameteri(
		               GL_TEXTURE_2D,
		               GL_TEXTURE_MAG_FILTER, //����Ŵ�ʱ���˲�����
					   GL_LINEAR); // �����˲�
    }
    else
	  printf("����ͼ���ļ�����ʧ�ܣ����·���͸�ʽ��24bit��bmp��\n");
    //----------------------------------------------------------------------------
	// ����װ������ʱʹ�õ���ʱ������������
    if (TextureImage[0])        // �����Ƿ����
    {
       if (TextureImage[0]->data)      // ����ͼ���Ƿ����
        {
           free(TextureImage[0]->data);    // �ͷ�����ͼ��ռ�õ��ڴ�
        }
       free(TextureImage[0]);       // �ͷ�ͼ��ṹ
    }
	//----------------------------------------------------------------------------
    return texture_ID;    //����������������ID
}

//����2������ID��
GLuint groundTexture,  fenceTexture;

void init(void)
{ 
	glEnable(GL_DEPTH_TEST);   
	setLight(0);  //��Դλ��Ĭ��Ϊ0�ţ���ǰ����

	//������������groundTexture��
    groundTexture = LoadGLTextures("����ʯ.bmp");
	//groundTexture = LoadGLTextures("�ذ�.bmp");
	groundTexture = LoadGLTextures("�ݵ�.bmp");
	//����դ������FenceTexture��
	fenceTexture = LoadGLTextures("שǽ2.bmp");
	//fenceTexture = LoadGLTextures("Χ��.bmp");

	//����һ������(Ground)����ʾ�б�1,�μ�p.194�ϵ�5.6��
	glNewList(1,GL_COMPILE);
	  //glColor3f(0.0,1.0,0.0);
	  glBegin(GL_QUADS); 
	    //����ӳ�䣺��������ռ䣨�������꣩�����ռ䣨�������꣩�Ķ�Ӧ��ϵ��
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, -8.0f, 0.0f);   
        glTexCoord2f(0.0f, 4.0f); glVertex3f(-8.0f, 8.0f, 0.0f);   
        glTexCoord2f(4.0f, 4.0f); glVertex3f(8.0f, 8.0f, 0.0f);   
        glTexCoord2f(4.0f, 0.0f); glVertex3f(8.0f, -8.0f, 0.0f); 
      glEnd();
	glEndList();
	
	//����һ����դ����Fence������ʾ�б�2
	glNewList(2,GL_COMPILE);
	  //glColor3f(1.0,1.0,1.0);
	  glBegin(GL_QUADS);   
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, -8.0f, 0.0f);   
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-8.0f, -8.0f, 1.5f);   
        glTexCoord2f(8.0f, 1.0f); glVertex3f(8.0f, -8.0f, 1.5f);   
        glTexCoord2f(8.0f, 0.0f); glVertex3f(8.0f, -8.0f, 0.0f);   
      glEnd();
	glEndList();
}

void drawTeapot()
{	glColor3f(0.5,0.3,0.3);      //�趨���ɫ��
	glRotatef(90,1.0,0.0,0.0);   //��x����ת90��
	glTranslatef(1.0,0.75,-2.0);  //ƽ��  

	glutSolidTeapot(1.0);      //���Ʋ��
	
	glTranslatef(-1.0,-0.75,2.0); //ƽ�Ƹ�ԭ
	glRotatef(-90,1.0,0.0,0.0);  //��ת��ԭ������x����ת-90��(�Ա㲻Ӱ���̵Ļ���)
    glColor3f(1.0,1.0,1.0);      //�ָ���ɫ
}

GLdouble  eyex=2,eyey=5,eyez=2;  //�ӵ�ĳ�ʼλ������(1,5,5)

void display(void)
{
	  // �����Ļ   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
   
    // �����ӽ�   
    glMatrixMode(GL_PROJECTION);   // ѡ��ͶӰģ�Ӿ���
    glLoadIdentity();   
	//����͸��ͶӰ��
    gluPerspective(75, //75����Ұ�Ƕ�
		                       1, //�ݺ�ȣ�1��1��
					         0.1, //��Ȳü��浽����Ľ�����
					      1000  //��Ȳü��浽�����Զ����
				             );   
    glMatrixMode(GL_MODELVIEW);   
    glLoadIdentity(); 
	//�趨������ӵ㣩��λ�ã�
    gluLookAt(eyex,eyey,eyez,  //�ӵ�E(eyex,eyey,eyez) 
			          0,0,0,  //�ο���C(centerx,centery,centerz)
			          0,0,1   //����������Ϸ���U(upx,upy,upz)
			          );   
    drawTeapot();  //�����

	glEnable(GL_TEXTURE_2D);  //����GL_TEXTURE_2D����
	
	//5.�󶨡�groundTexture���������ڻ��Ƶ���   
    glBindTexture(GL_TEXTURE_2D, groundTexture); 
	glCallList(1); 	    
	//5.�󶨡�fenceTexture���������ڻ���դ��   
    glBindTexture(GL_TEXTURE_2D, fenceTexture); 
    glCallList(2);  

    // �� z ����ת-90�㡢-180�㡢-270����ٻ���3��դ����
    glRotatef(-90, 0, 0, 1);   
    glCallList(2); 
	glRotatef(-180, 0, 0, 1);   
    glCallList(2); 
	glRotatef(-270, 0, 0, 1);   
    glCallList(2); 
	
	glDisable(GL_TEXTURE_2D); //�ر�GL_TEXTURE_2D����
	 
    glFlush();  //ˢ��
}


//��Ӧ���̰����Ļص�����
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	if (uchKey == 27/*Esc*/) {
		exit(0);
	}
		//��Դλ�ÿ��ƣ�
	if (uchKey >='0' && uchKey <='4') {
	  setLight(uchKey-'0');
	}
	glutPostRedisplay(); 
}

void Special(int iKey, int iXPos, int iYPos)
{
	if(iKey == 100/*Left*/) {
		eyex -= 0.5;  //eyey += 1; 
	}
	if(iKey == 101/*Up*/) {
		eyez -= 0.5;
	}
	if(iKey == 102/*Right*/) {
	    eyex += 0.5; // eyey -= 1;
	}
	if(iKey == 103/*Down*/) {
		eyez += 0.5;
	}
	glutPostRedisplay(); 
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(400, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("����Ӧ��ʾ����Esc���˳���");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(Keyboard); //���̰���ʱ���õĻص�����
	glutSpecialFunc(Special); //���ⰴ���Ļص�����
	glutMainLoop();
	return 0;
}