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

//动态分配内存__首次适应算法

#ifndef ____MOMORY_FIRST__C_______
#define ____MOMORY_FIRST__C_______

#include "memory.h"

#include "error.h"

#if defined( MEMORY_FIRST )

#else if defined (MEMORY_BITMAP)


#else if defined (MEMORY_LIST)


#endif 

//初始化空间段
void Init(MEM &L,int size) 
{
     MEM p = new LNode;
     p->size = size;
     p->state = 0;
     p->task_name = 'n';
     p->next = NULL;
     L->next = p;
}

//找出连续的空闲资源,回收空闲空间
void setfree(MEM &L) 
{
     MEM p=L->next,q=p->next;
     while(p && q)
     {
         if(p->state == 0 && q->state == 0) //如果空间连续,则回收
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
     cout<<"回收成功"<<endl;
}


//删除作业
void deltask(const MEM L) 
{
     char task_name;
     int flag=0;
     Display(L);
     cout<<"请输入需要回收的作业名称:";
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
             q = q->next;   //找到要删除作业的下一个结点
         }
     }
     if(flag == 0)
         cout<<"删除作业不成功"<<endl;
     else
         cout<<"删除作业成功"<<endl;
}


//显示作业
void Display(const MEM L) 
{
     int count=1;
     MEM p = L->next;
     cout<<"结点号 作业 状态 大小"<<endl;
     while(p)
     {
         cout<<"结点"<<count<<"     "<<p->task_name<<"    ";
         cout<<p->state<<"    "<<p->size<<endl;
         p = p->next;
         count++;
     }
}


//添加作业
void Add(MEM &L) 
{
     int new_size;
     char new_name;
     MEM q=L,p = L->next;
     cout<<"请输入新任务的名称:";
     cin>>new_name;
     cout<<"请输入新任务的大小:";
     cin>>new_size;
    
     while(p) //查找空闲资源进行分配
     {
         if (new_size<=0)
         {
             cout<<endl<<"申请的空间不能小于1"<<endl;
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
             break;   //分配完成便退出
         }
         else
         {
             p = p->next;   //移动到足够分配的空结点
         }
         if(!p)
         {
             cout<<"作业"<<new_name<<"内存分配不成功"<<endl;
             break;
         }
     }
     p = L->next;
     while(p) //删除大小为0的结点,当分配空间完时会出现0结点
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


//选择操作类型
void choice(MEM &L)   
     { 
       int choice;
       do
       {
         cout<<"0.退出本程序"<<endl;
         cout<<"1.添加新的作业"<<endl;
         cout<<"2.显示当前作业"<<endl;        
         cout<<"3.删除一条作业"<<endl;
         cout<<"4.回收空闲空间"<<endl;
         cout<<endl<<"输入你的选择:";
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
             cout<<"请输入正确的选择!"<<endl;
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
     cout<<"初始多大空间,请输入一个整数:"<<endl;
     cin>>N;
     Init(L,N);     //初始化大小为1000的内存空间
     choice(L);     //进入操作
}


//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif //____MOMORY_FIRST__C_______
