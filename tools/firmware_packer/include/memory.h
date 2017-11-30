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
// File    : memory.h                                                                                       //
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

#ifndef ____MOMORY_FIRST__H_______
#define ____MOMORY_FIRST__H_______	1


#if defined(MEMORY_FIRST)// defined(MEMORY_FIRST)

typedef struct LNode
{     
	int size;         	//�ڴ��С
    int state;    		//0��ʾ���У�1��ʾ�Ѿ�װ����ҵ
    char task_name;   	//װ�����ҵ����
    struct LNode *next;
}LNode,*MEM;


//��ʼ���ռ�Σ���С���Զ���
void Init(MEM &L,int size);

//ѡ�����ҵ�Ĳ������ͣ�ѡ0�˳�������1�½���ҵ��
//����Ѿ�����Ϳ�����Դ�����ѡ3ɾ��ĳ��ҵ��ѡ4������Դ��
void choice(MEM &L);

//��ӵ���ҵ��
void Add(MEM &L);

//����Ѿ�����Ϳ��н�㡣
void Display(const MEM L);

//����Ѿ�����Ϳ�����Դ�����
void deltask(const MEM L);

//���տ��пռ䣬�����������Ŀ��н����ϲ���
void setfree(MEM &L);


#else
/*


#else if defined (MEMORY_BITMAP)

#else if defined (MEMORY_LIST)
*/
/*
����MCB���ڴ�����㷨
��������㷨�������ѵķ���ͻ�����һ�����������ơ���������ÿ���ڵ���һ��MCB���һ���ڴ����ɡ����MCB������
�����������ڴ���ʹ��״����
MCB��Ľṹ���£�
*/

typedef struct St_MemControlBlock
{
	struct St_MemControlBlock * pNext;
	unsigned nLimit;
	unsigned uSignature;
}St_MemControlBlock;
/*
��ϵͳ��ʼ����ʱ�������Ѿ���MCB���ĵ�һ���ڵ㡣����ڵ��MCB����һ�����п飬�����������ѵĴ�С��
��һ���ڴ汻�����ʱ��malloc�������������MCB������ͼ�ҵ�һ���㹻��Ŀ��п顣���û���ҵ����ͷ���NULL�����
�ҵ�����ͨ��MCB�����п�Ĵ�С�����������п�Ĵ�С������Ҫ������ڴ��С�����������п��Ϊʹ��״̬������
����ڵ�MCB�����ڴ��ַ��Ϊ�׵�ַ���ء����������п�Ĵ�С������Ҫ������ڴ��С����ֲ������Ϊ�����ڵ㡣
�ѵ�һ���ڵ��Ϊʹ��״̬���ѵڶ����ڵ��Ϊ����״̬��Ȼ�󣬷��ص�һ���ڵ�MCB�����ڴ��ַ��
Ϊ�ӿ�����ٶȣ����Զ�һЩ�������������Ż������磬���ԶԶ�ջ��ʽ���ȷ�����ͷţ��ķ���ʹ�ý����Ż���
��һ���ڴ汻�ͷ�ʱ��free����������ڴ���Ϊ����״̬���������ڴ���һ�����п����ڣ�free�������ϲ�����������
��Ϊһ���顣
*/

#endif 

//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif //____MOMORY_FIRST__H_______




