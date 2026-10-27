#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeKLists(struct ListNode** lists, int k) {
    struct ListNode dummy;
    struct ListNode* curr = &dummy;
    int i, minIndex;

    dummy.next = NULL;

    struct ListNode** heads = malloc(k * sizeof(struct ListNode*));
    for(i = 0; i < k; i++) {
        heads[i] = lists[i];
    }

    while(1) {
        minIndex = -1;
        for(i = 0; i < k; i++) {
            if(heads[i] != NULL) {
                if(minIndex == -1 || heads[i]->val < heads[minIndex]->val) {
                    minIndex = i;
                }
            }
        }

        if(minIndex == -1) {
            break;
        }

        curr->next = heads[minIndex];
        curr = curr->next;
        heads[minIndex] = heads[minIndex]->next;
    }

    free(heads);
    return dummy.next;
}
