#include <stddef.h>
#include <stdlib.h>

#include "quick.h"

//以 linux api 建構，實作 middle of three

struct list_head *findMiddle(struct list_head *a,
                             struct list_head *b,
                             struct list_head *c,
                             void *priv,
                             list_cmp_func_t cmp)
{
    int cmpAB = cmp(priv, a, b);
    int cmpAC = cmp(priv, a, c);   
    int cmpBC = cmpAB - cmpAC;
 
    if ( (cmpAB <= 0 && cmpAC >= 0) || (cmpAB >= 0 && cmpAC <= 0) ) {               
        return a;
    }
    if ( (cmpAB >= 0 && cmpBC >= 0) || (cmpAB <= 0 && cmpBC <= 0)){                       
        return b;
    }
    return c;
}

void quick_sort_mid(void *priv, struct list_head *head, list_cmp_func_t cmp)
{
    int n = q_size(head);
    int i = 0;
    int max_level = 2 * n;
    struct list_head **begin = malloc(sizeof(struct list_head *) * max_level);
    LIST_HEAD(result);

    begin[0] = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(begin[0]);
    list_splice_tail_init(head, begin[0]);

    while (i >= 0) {
        if (!list_is_singular(begin[i]) && !list_empty(begin[i])) {
            /*not singular*/
            LIST_HEAD(left);
            LIST_HEAD(right);
            LIST_HEAD(tmp);

            struct list_head *pivot;
            if (begin[i]->next->next->next != begin[i]) {
                struct list_head *first = begin[i]->next;
                struct list_head *last = begin[i]->prev;
                struct list_head *mid = begin[i]->next;

                for (struct list_head *fast = begin[i]->next;
                     fast != begin[i] && fast->next != begin[i];
                     fast = fast->next->next) {
                    mid = mid->next;
                }
                pivot = findMiddle(first, mid, last, priv, cmp);
            } else {
                pivot = begin[i]->next;
            }

            list_del(pivot);  //將 pivot 從原有的list去除

            while (!list_empty(begin[i])) {
                struct list_head *cur = begin[i]->next;
                if (cmp(priv, cur, pivot) >= 0) {
                    list_move_tail(cur, &right);
                } else {
                    list_move_tail(cur, &left);
                }
            }

            begin[i + 1] = malloc(sizeof(struct list_head));
            INIT_LIST_HEAD(begin[i + 1]);

            begin[i + 2] = malloc(sizeof(struct list_head));
            INIT_LIST_HEAD(begin[i + 2]);

            list_splice_tail_init(&left, begin[i]);
            list_splice_tail_init(&right, begin[i + 2]);

            list_add(pivot, &tmp);
            list_splice_tail_init(&tmp, begin[i + 1]);

            i += 2;
        } else {
            /*singular*/
            if (!list_empty(begin[i])) {
                /* not empty*/
                struct list_head *pivot = begin[i]->next;
                list_add(pivot, &result);

                free(begin[i]);
                begin[i] = NULL;
            }
            i--;
        }
    }
    list_splice_tail_init(&result, head);
    free(begin);
}