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
	int size;         	//内存大小
    int state;    		//0表示空闲，1表示已经装入作业
    char task_name;   	//装入的作业名称
    struct LNode *next;
}LNode,*MEM;


//初始化空间段，大小可自定。
void Init(MEM &L,int size);

//选择对作业的操作类型，选0退出程序，先1新建作业，
//输出已经分配和空闲资源情况，选3删除某作业，选4回收资源。
void choice(MEM &L);

//添加的作业。
void Add(MEM &L);

//输出已经分配和空闲结点。
void Display(const MEM L);

//输出已经分配和空闲资源情况。
void deltask(const MEM L);

//回收空闲空间，若存在连续的空闲结点则合并。
void setfree(MEM &L);


#else
/*


#else if defined (MEMORY_BITMAP)

#else if defined (MEMORY_LIST)
*/
/*
基于MCB的内存管理算法
基于这个算法，整个堆的分配和回收由一个链表来控制。这个链表的每个节点由一个MCB块和一块内存块组成。这个MCB链描述
了所有所有内存块的使用状况。
MCB块的结构如下：
*/

typedef struct St_MemControlBlock
{
	struct St_MemControlBlock * pNext;
	unsigned nLimit;
	unsigned uSignature;
}St_MemControlBlock;
/*
当系统初始化的时候，整个堆就是MCB链的第一个节点。这个节点的MCB块是一个空闲块，描述了整个堆的大小。
当一块内存被分配的时候，malloc函数会搜索这个MCB链，试图找到一个足够大的空闲块。如果没有找到，就返回NULL。如果
找到，则通过MCB检查空闲块的大小。如果这个空闲块的大小等于需要分配的内存大小，则把这个空闲块标为使用状态，并把
这个节点MCB块后的内存地址作为首地址返回。如果这个空闲块的大小大于需要分配的内存大小，则分拆这个块为两个节点。
把第一个节点标为使用状态，把第二个节点标为空闲状态。然后，返回第一个节点MCB块后的内存地址。
为加快分配速度，可以对一些分配特征进行优化。例如，可以对堆栈方式（先分配后释放）的分配使用进行优化。
当一块内存被释放时，free函数把这个内存块标为空闲状态。如果这个内存块和一个空闲块相邻，free函数将合并这两个空闲
块为一个块。
*/

#endif 

//------------------------------------------------------------------------------------------------------------
// THE END !
//------------------------------------------------------------------------------------------------------------

#endif //____MOMORY_FIRST__H_______




