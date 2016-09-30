#include <GL/glut.h>	
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "line.h"

//ʵ��DrawPolygon��������ɶ���ε�ɨ��ת���㷨
//ʵ�ַ���߱�ͻ�߱�����ݽṹ����Ӧ�㷨
//����������g_iCtrlPntNum����������ֵ����g_pCtrlPoints�����У������Ƕ�ά�㣬���Ժ���z����
//!!!ע��!!!����M������ѡ������һ������ ���� ��һ�����㣬
//��g_pCtrlPoints[g_iCtrlPntNum-1] �� g_pCtrlPoints[0]����ͬ��
//���ı���Ϊ����g_iCtrlPntNum=5�����g_pCtrlPoints[0]���յ�g_pCtrlPoints[4]ָͬһ����

typedef struct edge 
{
	int    y;   //�ߵ��϶˵��y���꣨y_max��
	double x;   //���뵱ǰɨ���߽����x����
	double dx;  //x������
	struct edge * next;
} edge ;

//����߱�NET�ĳ�ʼ����
void init_edge_list (edge *edge_header,    //ָ��Ҫ������NET��ͷָ��
	                 int g_iCtrlPntNum,     //����εĶ�����
					 wcPt3D	g_pCtrlPoints[] //����εĶ�����������
                    )					
{
	int i;
	wcPt3D start_point, end_point;  //ĳ�ߵ������յ�
	wcPt3D *up,*down;  //ָ��ĳ�ߵ��϶˵���¶˵��ָ��
	edge *bak, *header ;
	
	int prev_trend,
		current_trend;	//����������ߣ���ֵΪ1���������ߣ���ֵΪ-1�����������Ĵ���

	if (g_iCtrlPntNum<3)	//������̫�٣��޷����ɶ����
		return;

	//�ж����һ���ߣ�����һ���ߵ���һ���ߣ�������(��Ϊ����εı߹��ɱջ�)
	if (g_pCtrlPoints[g_iCtrlPntNum-1].y > g_pCtrlPoints[g_iCtrlPntNum-2].y )
		prev_trend = 1;   //��һ����������
	else if (g_pCtrlPoints[g_iCtrlPntNum-1].y < g_pCtrlPoints[g_iCtrlPntNum-2].y )
		prev_trend = -1;  //��һ����������
	else 
		prev_trend = 0;   //��һ����ˮƽ��
	
	for (i=0;i<g_iCtrlPntNum-1;i++)
	{
		start_point = g_pCtrlPoints[i];
		end_point   = g_pCtrlPoints[i+1];
	    //���㱾���ߵĳ���current_trend
		if ( g_pCtrlPoints[i].y == g_pCtrlPoints[i+1].y )  
			continue;	//ˮƽ�ߣ�������������һ���ߵĴ���
		else 
		if ( g_pCtrlPoints[i+1].y > g_pCtrlPoints[i].y ) { //���ϱ�
			   up = &end_point;
			   down = &start_point;
			   current_trend = 1; 
		}
		else  {  //���±�
		   up = &start_point;
		   down = &end_point;
		   current_trend = -1;
		}
		//�������һ�������ݵĶ�̬����:
		edge * new_edge = (edge * )malloc(sizeof(edge));
		//����ʼ��:
		new_edge->y = up->y;   //y_max
		new_edge->x = down->x; //x_min
		new_edge->dx = double(up->x - down->x) / double(up->y - down->y); //б�ʵĵ���
		new_edge->next = NULL;
		//��������㣺
		if (prev_trend == current_trend ) //2�߶����Ӵ���ɨ���ߵĽ���ֻ��1��
		{
			if (current_trend == 1) //�߶�����Ϊ����
			{
				//�߶��¶˵�y����һ��ɨ����,����NET[y+1]�ࣺ
				new_edge->x = new_edge->x + new_edge->dx;
				header = &edge_header[(int)down->y+1];  
			}
			else if (current_trend == -1) //�߶�����Ϊ����
			{
				//�߶��¶˵㲻�䣬�϶˵��½�һ��ɨ����
				new_edge->y = new_edge->y-1;
				header= &edge_header[(int)down->y]; //����NET[y]��
			}
		}
		else //��ֵ�㣬2�߶����Ӵ���ɨ���ߵĽ���ֻ��2��
			header= &edge_header[(int)down->y];
		//��ԭ���߶ε�y���ֻΪ1�������������Ϊˮƽ��:
		if ( abs(down->y - new_edge->y) <0.1 )	
		{
			free(new_edge);  
			continue;
		}

		prev_trend = current_trend; 
	    //��x�ĵ������ƶ�bak��headerָ�룬ʹ������ָ����ȷ����new_edge��ǰ/��һ��λ��
		while (1) {
			bak = header;
			header = header->next;
			if (header==NULL)
				break;
			if ( header->x > new_edge->x  ||
				 ( header->x == new_edge->x  &&	header->dx >= new_edge->dx) )
				break;
		}
		//��new_edge����NET[y]�ࣺ
		bak->next = new_edge;
		new_edge->next = header;
	} //end of ��for (i=0;i<g_iCtrlPntNum-1;i++)����������һ����
}

//��NET[y]�еıߣ���x�ĵ�����ϲ���AET�У�
void merge_two_list( edge * cur,    //��ֵΪAET����ʼ��ַ
	                 edge * first,  //��ֵΪNET[y].next
					 edge * second  //��ֵΪAET.next
				   )
{
	while (first!=NULL && second!=NULL)
	{
		if (first->x < second->x
			|| (first->x == second->x && first->dx < second->dx)
			)  //��first��ָ�ı߲���AET
		{
			cur->next = first;
			cur = first;
			first = first->next; //ȡNET[y]�е���һ����
		}
		else { //����second��curָ�룬ֱ�����㡰first->x < second->x��
			cur->next = second;
			cur = second;
			second = second->next;
		}
	}
	if (first!=NULL)
		cur->next = first;
	if (second!=NULL)
		cur->next = second;
}

//����header��ָ��AET��
void Resort(edge *header)
{
	int i,len=0;
	edge *p,*selected,*bak_sel,*q, *bak_q;

	//��ȡ��ĳ���
	for (p=header->next;p!=NULL;p=p->next)
		len++;
	
	p = header;
	
	//��len-1������
	for (i=0;i<len-1;i++)
	{
		bak_sel = p;
		selected =p->next;

		for (bak_q=selected,q=selected->next;
			 q!=NULL;
			 bak_q=q,q=q->next)
		{
			if (q->x < selected->x || (q->x == selected->x && q->dx < selected->dx) )
			{
				bak_sel = bak_q;
				selected = q;
			}
		}

		bak_sel ->next = selected->next;
		selected->next = p->next;
		p->next = selected;
		p = p->next;
	}
}

void DrawPolygon(int g_iCtrlPntNum, wcPt3D	g_pCtrlPoints[])
{
	int i,y;
	edge edge_header[defSCREEN_HEIGHT];  //NET����
	edge active_edge_list;  //AET��ͷ���

	
	//��ʼ������߱�
	active_edge_list.next = NULL;
	for(y = 0; y < defSCREEN_HEIGHT; ++y)
		edge_header[y].next = NULL;
	init_edge_list(edge_header,g_iCtrlPntNum,g_pCtrlPoints);


	for (y=0;y<defSCREEN_HEIGHT;y++)
	{
		//����yɨ���ߵķ���߱�ǿգ���ȡ�²����߱���
		if(edge_header[y].next != NULL)
		{
			merge_two_list(&active_edge_list,edge_header[y].next,active_edge_list.next);
		}

		//���¶Ի�߱��������
		Resort(&active_edge_list);

		//�����ɨ���������εĽ��㲢���
		edge *p = active_edge_list.next;

		p = active_edge_list.next;
		while( p != NULL && p->next != NULL )  
		{  
			for(float j = p->x;j <= p->next->x; j++)  
			{  
				//drawpixel(static_cast<int>(j),y,(0xff<<16));   //������ɫ
				drawpixel((int)j,y,(0xff<<16));
   
			}
			p = p->next->next;//���Ƕ˵����  
		}
		

		//��AET��ɾ��ymax=��ǰɨ����y�ı�
		edge *q = &active_edge_list;
		p = q->next;
		while(p != NULL)
		{
			if(p->y == y)
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
			else
			{
				q = q->next;
				p = q->next;
			}
		}
		
		//����AET��ʣ�±ߵ�xֵ����б�ʵĵ���)
		p = active_edge_list.next;
		while( p != NULL )  
		{  
			p->x += p->dx;
			p = p->next;  
		}
	}
}
   

