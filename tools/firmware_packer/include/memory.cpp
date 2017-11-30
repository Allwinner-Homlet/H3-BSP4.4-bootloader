//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//    I BBMMBB      BBMMBBI     BBI EEMMFFI BBMMI     BBI EEMMFFI BBMMI       BBMMBBI   EEMMI EEMMLL        //
//    EEI   EEFF  EEI   LLBB  EEMMBBI I BBEE  I EE  EEMMBBI I BBEE  I EE    EEI   LLBB    EEBBI I BB        //
//  I FF        I EE      BBI   EEI     EEI   I EE    EEI     EEI   I EE  I EE      BBI   EEI   I EE        //
//  LLFF        LLEE      EELL  EEI     EEI   I EE    EEI     EEI   I EE  LLEE      EELL  EEI   I EE        //
//  I EE      I I BB      EEI   EEI     EEI   I EE    EEI     EEI   I EE  I BB      EEI   EEI   I EE        //
//    BBLL  I I   BBFF  I EE    EEI     EEI   I BB    EEI     EEI   I BB    BBFF  I EE    BBI   I EE        //
//    I BBMMEE    I BBMMBB    EEMMMMLLBBMMBBLLMMMMEEEEMMMMLLBBMMBBLLMMMMEE  I BBMMBB    EEMMMMLLBBMMBB      //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
//                                                Scott System                                              //
//                                                                                                          //
//                               (c) Copyright 2006-2007, Scottyu China                                     //
//                                                                                                          //
//                                           All Rights Reserved                                            //
//                                                                                                          //
// File    : memory.cpp                                                                                     //
// By      : scottyu                                                                                        //
// Version : V1.00                                                                                          //
// Time    : 2007-12-13 15:50:23                                                                            //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//
//                                                                                                          //
// HISTORY                                                                                                  //
//                                                                                                          //
// 1 2007-12-13 15:50:23                                                                                    //
//                                                                                                          //
//                                                                                                          //
//                                                                                                          //
//----------------------------------------------------------------------------------------------------------//

//��̬�����ڴ�__�״���Ӧ�㷨

#ifndef ____MOMORY_FIRST__C_______
#define ____MOMORY_FIRST__C_______

#include "memory.h"

#include "error.h"

#if defined( MEMORY_FIRST )

#else if defined (MEMORY_BITMAP)


#else if defined (MEMORY_LIST)


#endif 

//��ʼ���ռ��
void Init(MEM &L,int size) 
{
     MEM p = new LNode;
     p->size = size;
     p->state = 0;
     p->task_name = 'n';
     p->next = NULL;
     L->next = p;
}

//�ҳ������Ŀ�����Դ,���տ��пռ�
void setfree(MEM &L) 
{
     MEM p=L->next,q=p->next;
     while(p && q)
     {
         if(p->state == 0 && q->state == 0) //����ռ�����,�����
         {
             p->size +=q->size;
             p->next = p->next->next;
             delete q;
             q=p->next;
         }
         else
         {
             p = q;
             q = q->next;
         }
     }
     cout<<"���ճɹ�"<<endl;
}


//ɾ����ҵ
void deltask(const MEM L) 
{
     char task_name;
     int flag=0;
     Display(L);
     cout<<"��������Ҫ���յ���ҵ����:";
     cin>>task_name;

     MEM p=L,q=L->next;
     while(q)
     {
         if(q->task_name == task_name)
         {
             q->state=0;
             q->task_name='?';
             flag=1;
             break;
         }
         else
         {
             p = q;
             q = q->next;   //�ҵ�Ҫɾ����ҵ����һ�����
         }
     }
     if(flag == 0)
         cout<<"ɾ����ҵ���ɹ�"<<endl;
     else
         cout<<"ɾ����ҵ�ɹ�"<<endl;
}


//��ʾ��ҵ
void Display(const MEM L) 
{
     int count=1;
     MEM p = L->next;
     cout<<"���� ��ҵ ״̬ ��С"<<endl;
     while(p)
     {
         cout<<"���"<<count<<"     "<<p->task_name<<"    ";
         cout<<p->state<<"    "<<p->size<<endl;
         p = p->next;
         count++;
     }
}


//�����ҵ
void Add(MEM &L) 
{
     int new_size;
     char new_name;
     MEM q=L,p = L->next;
     cout<<"�����������������:";
     cin>>new_name;
     cout<<"������������Ĵ�С:";
     cin>>new_size;
    
     while(p) //���ҿ�����Դ���з���
     {
         if (new_size<=0)
         {
             cout<<endl<<"����Ŀռ䲻��С��1"<<endl;
             break;
         }
         if(p->state==0 && p->size >= new_size) 
         {   
         //****************************************************//
             MEM q = new LNode;
             q->size = p->size - new_size; 
             q->state = 0;
             q->task_name='?';
             q->next=NULL;
         //****************************************************//
             p->size = new_size;
             p->state = 1;
             p->task_name=new_name;
             q->next = p->next;
             p->next = q;
             break;   //������ɱ��˳�
         }
         else
         {
             p = p->next;   //�ƶ����㹻����Ŀս��
         }
         if(!p)
         {
             cout<<"��ҵ"<<new_name<<"�ڴ���䲻�ɹ�"<<endl;
             break;
         }
     }
     p = L->next;
     while(p) //ɾ����СΪ0�Ľ��,������ռ���ʱ�����0���
     {
         if(p->size == 0)
         {
             q->next = q->next->next;
             delete p;
             p = q->next;
         }
         else 
         {
             q = p;
             p = p->next;
         }
     }
}


//ѡ���������
void choice(MEM &L)   
     { 
       int choice;
       do
       {
         cout<<"0.�˳�������"<<endl;
         cout<<"1.����µ���ҵ"<<endl;
         cout<<"2.��ʾ��ǰ��ҵ"<<endl;        
         cout<<"3.ɾ��һ����ҵ"<<endl;
         cout<<"4.���տ��пռ�"<<endl;
         cout<<endl<<"�������ѡ��:";
         cin>>choice;
         switch(choice)
         {
         case 0:
             exit(1);break;
         case 1:
             Add(L); break;
         case 2:
             Display(L); break;        
         case 3:
             deltask(L); break;
         case 4:
             setfree(L); break;
         default:
             cout<<"��������ȷ��ѡ��!"<<endl;
             break;
         }
         cout<<endl;
       }while(choice!=0 ||choice!=1 || choice !=2 || choice!=3 || choice!=4);
}
 
 
#include <iostream.h>
#include <stdlib.h>


void main()
{
     MEM L=new LNode; //MEM
     int N;
     cout<<"��ʼ���ռ�,������һ������:"<<endl;
     cin>>N;
     Init(L,N);     //��ʼ����СΪ1000���ڴ�ռ�
     choice(L);     //�������
}


//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif //____MOMORY_FIRST__C_______
