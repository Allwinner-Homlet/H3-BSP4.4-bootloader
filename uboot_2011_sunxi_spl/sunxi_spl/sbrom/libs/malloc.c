/*
**********************************************************************************************************************
*											        eGon
*						           the Embedded GO-ON Bootloader System
*									       eGON memory sub-system
*
*						  Copyright(C), 2006-2010, SoftWinners Microelectronic Co., Ltd.
*                                           All Rights Reserved
*
* File    : mem_alloc
*
* By      : Jerry
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/

#include "common.h"

struct alloc_struct_t
{
    __u32 address;                      //�����ڴ�ĵ�ַ
    __u32 size;                         //������ڴ��С���û�ʵ�ʵõ����ڴ��С
    __u32 o_size;                       //�û�������ڴ��С
    struct alloc_struct_t *next;
};

#define MY_BYTE_ALIGN(x)                ( ( (x + 15)/16) * 16)             /* alloc based on 1k byte */

static struct alloc_struct_t boot_heap_head, boot_heap_tail;
/*
*********************************************************************************************************
*                       CREATE HEAP
*
* Description: create heap.
*
* Aguments   : pHeapHead    heap start address.
*              nHeapSize    heap size.
*
* Returns    : EPDK_OK/EPDK_FAIL.
*********************************************************************************************************
*/

__s32 create_heap(__u32 pHeapHead, __u32 nHeapSize)
{
    boot_heap_head.size    = boot_heap_tail.size = 0;
    boot_heap_head.address = pHeapHead;
    boot_heap_tail.address = pHeapHead + nHeapSize;
    boot_heap_head.next    = &boot_heap_tail;
    boot_heap_tail.next    = 0;

    return 0;
}

/*
*********************************************************************************************************
*                       MALLOC BUFFER FROM HEAP
*
* Description: malloc a buffer from heap.
*
* Aguments   : num_bytes    the size of the buffer need malloc;
*
* Returns    : the po__s32er to buffer has malloc.
*********************************************************************************************************
*/
void *malloc(__u32 num_bytes)
{
    struct alloc_struct_t *ptr, *newptr;
    __u32  actual_bytes;

    if (!num_bytes) return 0;

    actual_bytes = MY_BYTE_ALIGN(num_bytes);    /* translate the byte count to size of long type       */

    ptr = &boot_heap_head;                      /* scan from the boot_heap_head of the heap            */

    while (ptr && ptr->next)                    /* look for enough memory for alloc                    */
    {
        if (ptr->next->address >= (ptr->address + ptr->size +                                          \
                2 * sizeof(struct alloc_struct_t) + actual_bytes))
        {
            break;
        }
                                                /* find enough memory to alloc                         */
        ptr = ptr->next;
    }

    if (!ptr->next)
    {
        return 0;                   /* it has reached the boot_heap_tail of the heap now              */
    }

    newptr = (struct alloc_struct_t *)(ptr->address + ptr->size);
                                                /* create a new node for the memory block             */
    if (!newptr)
    {
        return 0;                               /* create the node failed, can't manage the block     */
    }

    /* set the memory block chain, insert the node to the chain */
    newptr->address = ptr->address + ptr->size + sizeof(struct alloc_struct_t);
    newptr->size    = actual_bytes;
    newptr->o_size  = num_bytes;
    newptr->next    = ptr->next;
    ptr->next       = newptr;

    return (void *)newptr->address;
}
/*
*********************************************************************************************************
*                       MALLOC BUFFER FROM HEAP
*
* Description: malloc a buffer from heap.
*
* Aguments   : num_bytes    the size of the buffer need malloc;
*
* Returns    : the po__s32er to buffer has malloc.
*********************************************************************************************************
*/
void *realloc(void *p, __u32 num_bytes)
{
    struct alloc_struct_t *ptr, *prev;
    void   *tmp;
    __u32  actual_bytes;

    if(!p)
    {
        return malloc(num_bytes);             /* ���������ָ���ǿգ����մ������ֽ��������ڴ�    */
    }
    if (!num_bytes)
    {
        return p;                                   /* �������׷�ӵ��ڴ��ֽ�����0����ֱ�ӷ��ص�ǰ��ָ�� */
    }

    ptr = &boot_heap_head;                          /* look for the node which po__s32 this memory block                   */
    while (ptr && ptr->next)
    {
        if (ptr->next->address == (__u32)p)
            break;                                  /* find the node which need to be release                              */
        ptr = ptr->next;
    }

    //��ʱ��ptrָ������û�ָ���ǰһ���ڵ�
    prev = ptr;
    ptr  = ptr->next;

    if(!ptr)
    {
        return 0;                                   /* ���û���ҵ��û������ĵ�ַ         */
    }
    //��ptrָ���û��ڵ�
    actual_bytes = MY_BYTE_ALIGN(ptr->o_size + num_bytes);
    if(actual_bytes == ptr->size)
    {
        return p;
    }

    tmp = malloc(actual_bytes);
    if(!tmp)
    {
        return 0;
    }
    memcpy(tmp, (void *)ptr->address, ptr->size);
    prev->next = ptr->next;     /* delete the node which need be released from the memory block chain  */

    return tmp;
}

/*
*********************************************************************************************************
*                       FREE BUFFER TO HEAP
*
* Description: free buffer to heap
*
* Aguments   : p    the po__s32er to the buffer which need be free.
*
* Returns    : none
*********************************************************************************************************
*/
void  free(void *p)
{
    struct alloc_struct_t *ptr, *prev;

	if( p == NULL )
		return;

    ptr = &boot_heap_head;                /* look for the node which po__s32 this memory block                     */
    while (ptr && ptr->next)
    {
        if (ptr->next->address == (__u32)p)
            break;              /* find the node which need to be release                              */
        ptr = ptr->next;
    }

	prev = ptr;
	ptr = ptr->next;

    if (!ptr) return;           /* the node is heap boot_heap_tail                                               */

    prev->next = ptr->next;     /* delete the node which need be released from the memory block chain  */

    return ;
}






