#include "link_list.h"

/**
 * @brief 初始化链表
 * @param list 链表指针
 */
void ll_init(LinkedList *list)
{
    if (list == NULL) return;
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/**
 * @brief 创建新节点
 * @param data 节点数据
 * @return 成功返回节点指针，失败返回NULL
 */
ListNode* ll_create(int data)
{
    ListNode *newNode = (ListNode*)malloc(sizeof(ListNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

/**
 * @brief 在链表头部插入节点
 * @param list 链表指针
 * @param data 要插入的数据
 * @return 成功返回true，失败返回false
 */
bool ll_insert_head(LinkedList *list, int data)
{
    if (list == NULL) return false;
    
    ListNode *newNode = createListNode(data);
    if (newNode == NULL) return false;
    
    newNode->next = list->head;
    list->head = newNode;
    
    // 如果是空链表，尾指针也要更新
    if (list->tail == NULL) {
        list->tail = newNode;
    }
    
    list->size++;
    return true;
}

/**
 * @brief 在链表尾部插入节点
 * @param list 链表指针
 * @param data 要插入的数据
 * @return 成功返回true，失败返回false
 */
bool ll_insert_tail(LinkedList *list, int data)
{
    if (list == NULL) return false;
    
    ListNode *newNode = createListNode(data);
    if (newNode == NULL) return false;
    
    if (list->tail == NULL) {
        // 空链表情况
        list->head = newNode;
        list->tail = newNode;
    } else {
        // 非空链表直接操作tail
        list->tail->next = newNode;
        list->tail = newNode;
    }
    
    list->size++;
    return true;
}

/**
 * @brief 在指定位置插入节点
 * @param list 链表指针
 * @param index 插入位置(0-based)
 * @param data 要插入的数据
 * @return 成功返回true，失败返回false
 */
bool ll_insert_index(LinkedList *list, size_t index, int data)
{
    if (list == NULL || index > list->size) return false;
    
    // 处理头部和尾部插入的特殊情况
    if (index == 0) return ll_insert_head(list, data);
    if (index == list->size) return ll_insert_tail(list, data);
    
    ListNode *newNode = createListNode(data);
    if (newNode == NULL) return false;
    
    // 找到插入位置的前一个节点
    ListNode *prev = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        prev = prev->next;
    }
    
    newNode->next = prev->next;
    prev->next = newNode;
    
    list->size++;
    return true;
}

/**
 * @brief 删除链表头部节点
 * @param list 链表指针
 * @return 成功返回被删除节点的数据，失败返回-1（需改进）
 */
int ll_delete_head(LinkedList *list)
{
    if (list == NULL || list->head == NULL) return -1;
    
    ListNode *temp = list->head;
    int data = temp->data;
    
    list->head = list->head->next;
    free(temp);
    
    // 如果删除后链表为空，更新tail
    if (list->head == NULL) {
        list->tail = NULL;
    }
    
    list->size--;
    return data;
}

/**
 * @brief 删除链表尾部节点
 * @param list 链表指针
 * @return 成功返回被删除节点的数据，失败返回-1
 */
int ll_delete_tail(LinkedList *list)
{
    if (list == NULL || list->tail == NULL) return -1;
    
    int data = list->tail->data;
    
    // 只有一个节点的情况
    if (list->head == list->tail) {
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
    } else {
        // 找到尾节点的前一个节点
        ListNode *prev = list->head;
        while (prev->next != list->tail) {
            prev = prev->next;
        }
        
        free(list->tail);
        prev->next = NULL;
        list->tail = prev;
    }
    
    list->size--;
    return data;
}

/**
 * @brief 删除第一个匹配值的节点
 * @param list 链表指针
 * @param data 要删除的数据
 * @return 成功返回true，失败返回false
 */
bool ll_delete_by_value(LinkedList *list, int data)
{
    if (list == NULL || list->head == NULL) return false;
    
    // 处理头节点匹配的情况
    if (list->head->data == data) {
        deleteAtHead(list);
        return true;
    }
    
    ListNode *prev = list->head;
    ListNode *current = list->head->next;
    
    while (current != NULL) {
        if (current->data == data) {
            prev->next = current->next;
            
            // 如果删除的是尾节点
            if (current == list->tail) {
                list->tail = prev;
            }
            
            free(current);
            list->size--;
            return true;
        }
        
        prev = current;
        current = current->next;
    }
    
    return false;
}

/**
 * @brief 获取指定位置的节点数据
 * @param list 链表指针
 * @param index 位置索引(0-based)
 * @param outData 输出参数，存储获取的数据
 * @return 成功返回true，失败返回false
 */
bool ll_get_index(LinkedList *list, size_t index, int *outData)
{
    if (list == NULL || outData == NULL || index >= list->size) return false;
    
    ListNode *current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    
    *outData = current->data;
    return true;
}

/**
 * @brief 查找数据是否存在
 * @param list 链表指针
 * @param data 要查找的数据
 * @return 存在返回true，不存在返回false
 */
bool ll_contains(LinkedList *list, int data)
{
    if (list == NULL) return false;
    
    ListNode *current = list->head;
    while (current != NULL) {
        if (current->data == data) {
            return true;
        }
        current = current->next;
    }
    
    return false;
}

/**
 * @brief 打印链表内容
 * @param list 链表指针
 */
void ll_print(LinkedList *list)
{
    if (list == NULL) {
        printf("Invalid list\n");
        return;
    }
    
    printf("LinkedList[%zu]: ", list->size);
    ListNode *current = list->head;
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" -> ");
        }
        current = current->next;
    }
    printf("\n");
}

/**
 * @brief 清空链表，释放所有节点
 * @param list 链表指针
 */
void ll_clear(LinkedList *list)
{
    if (list == NULL) return;
    
    ListNode *current = list->head;
    while (current != NULL) {
        ListNode *temp = current;
        current = current->next;
        free(temp);
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}
