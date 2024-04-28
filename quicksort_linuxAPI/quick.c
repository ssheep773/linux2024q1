#include <stddef.h>
#include <stdlib.h>

#include "quick.h"

//以 linux api 建構
// 目的： create begin by link-list 
typedef struct begin_t{
    struct list_head blist;
    struct list_head *head;
} begin_t;

static inline begin_t *new_begin(struct list_head *head)
{
    begin_t *new_node = malloc(sizeof(begin_t)); // create new begin_t node 
    if (!new_node)
        return NULL;

    new_node->head = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(new_node->head);
    
    list_splice_tail_init(head, new_node->head);  // init 必要

    INIT_LIST_HEAD(&new_node->blist);
    return new_node;
}

void quick_sort(void *priv, struct list_head *head, list_cmp_func_t cmp)
{
    int count = 0;
    int max_count = 0;
    LIST_HEAD(result);

    struct list_head *begin_head = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(begin_head);

    begin_t *all = new_begin(head);
    list_add_tail(&all->blist, begin_head);

    struct list_head *sptr = begin_head->next;
   
    while (sptr != begin_head)
    {
        begin_t *snode = list_entry(sptr, begin_t, blist);
        if (!list_is_singular(snode->head) && !list_empty(snode->head)){
            /*not singular*/
            LIST_HEAD(left);
            LIST_HEAD(right);
            LIST_HEAD(tmp);

            struct list_head *pivot = snode->head->next;
            list_del(pivot);                //將 pivot 從原有的list去除    
            list_add(pivot, &tmp); 
            
            while(!list_empty(snode->head)){
                struct list_head *cur = snode->head->next;
                if (cmp(priv,cur ,pivot) >= 0 ){  // if not equal the sorting will be unstable
                    list_move_tail(cur, &right);
                } else {
                    list_move_tail(cur, &left);   
                }
            }
            list_splice_tail_init(&left, snode->head);

            begin_t *mid_begin = new_begin(&tmp);
            list_add_tail(&mid_begin->blist, begin_head);

            begin_t *right_begin = new_begin(&right);
            list_add_tail(&right_begin->blist, begin_head);

            sptr = sptr->next->next;
            count += 2;
            max_count = count > max_count ? count : max_count;
        } else { 
            /*singular*/
            if (!list_empty(snode->head)){
                /* not empty*/
                list_add(snode->head->next, &result);
            }
                begin_t *del = snode;
                sptr = sptr->prev;
                count --;

                list_del(&del->blist);
                free(del->head);
                free(del);             
        }
    }
    printf("max begin count = %d\n", max_count);
    free(begin_head);
    list_splice_tail_init(&result, head);
}