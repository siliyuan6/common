#ifndef LINK_LIST_H
#define LINK_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 链表节点结构
typedef struct ListNode {
    int data;               // 节点数据
    struct ListNode *next;  // 指向下一个节点的指针
} ListNode;

// 链表管理结构
typedef struct {
    ListNode *head;         // 链表头节点
    ListNode *tail;         // 链表尾节点
    size_t size;            // 链表长度
} LinkedList;

/**
 * @brief 初始化链表
 * @param list 链表指针
 */
void ll_init(LinkedList *list);
/**
 * @brief 创建新节点
 * @param data 节点数据
 * @return 成功返回节点指针，失败返回NULL
 */
ListNode* ll_create(int data);
/**
 * @brief 在链表头部插入节点
 * @param list 链表指针
 * @param data 要插入的数据
 * @return 成功返回true，失败返回false
 */
bool ll_insert_head(LinkedList *list, int data);
/**
 * @brief 在链表尾部插入节点
 * @param list 链表指针
 * @param data 要插入的数据
 * @return 成功返回true，失败返回false
 */
bool ll_insert_tail(LinkedList *list, int data);
/**
 * @brief 在指定位置插入节点
 * @param list 链表指针
 * @param index 插入位置(0-based)
 * @param data 要插入的数据
 * @return 成功返回true，失败返回false
 */
bool ll_insert_index(LinkedList *list, size_t index, int data);
/**
 * @brief 删除链表头部节点
 * @param list 链表指针
 * @return 成功返回被删除节点的数据，失败返回-1（需改进）
 */
int ll_delete_head(LinkedList *list);
/**
 * @brief 删除链表尾部节点
 * @param list 链表指针
 * @return 成功返回被删除节点的数据，失败返回-1
 */
int ll_delete_tail(LinkedList *list);
/**
 * @brief 删除第一个匹配值的节点
 * @param list 链表指针
 * @param data 要删除的数据
 * @return 成功返回true，失败返回false
 */
bool ll_delete_by_value(LinkedList *list, int data);
/**
 * @brief 获取指定位置的节点数据
 * @param list 链表指针
 * @param index 位置索引(0-based)
 * @param outData 输出参数，存储获取的数据
 * @return 成功返回true，失败返回false
 */
bool ll_get_index(LinkedList *list, size_t index, int *outData);
/**
 * @brief 查找数据是否存在
 * @param list 链表指针
 * @param data 要查找的数据
 * @return 存在返回true，不存在返回false
 */
bool ll_contains(LinkedList *list, int data);
/**
 * @brief 打印链表内容
 * @param list 链表指针
 */
void ll_print(LinkedList *list);
/**
 * @brief 清空链表，释放所有节点
 * @param list 链表指针
 */
void ll_clear(LinkedList *list);

#ifdef __cplusplus
}
#endif

#endif // LINK_LIST_H