#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    if (listsSize == 0) return NULL;
    if (listsSize == 1) return lists[0];

    struct ListNode* result = NULL;
    struct ListNode** llist = (struct ListNode**)malloc(listsSize * sizeof(struct ListNode*));
    for (int i = 0; i < listsSize; i++) {
        llist[i] = lists[i];
    }

    while (listsSize > 1) {
        listsSize--;
        int i = 0;
        while (i < listsSize) {
            if (i + 1 < listsSize) {
                if (llist[i]->val > llist[i + 1]->val) {
                    struct ListNode* temp = llist[i];
                    llist[i] = llist[i + 1];
                    llist[i + 1] = temp;
                    i++;
                }
            }
            i++;
        }
        struct ListNode* temp = llist[0];
        llist[0] = llist[1];
        llist[1] = temp;
        listsSize = 1;
    }

    result = llist[0];
    free(llist);
    return result;
}

void printList(struct ListNode* head) {
    struct ListNode* current = head;
    while (current != NULL) {
        printf("%d", current->val);
        if (current->next != NULL) printf(",");
        current = current->next;
    }
    printf("\n");
}

int main() {
    struct ListNode* lists[3] = {
        (struct ListNode*)malloc(sizeof(struct ListNode)), *(lists[0]) = (struct ListNode){1, (struct ListNode*)malloc(sizeof(struct ListNode))}, *(lists[0]->next) = (struct ListNode){4, (struct ListNode*)malloc(sizeof(struct ListNode))}, *(lists[0]->next->next) = (struct ListNode){5, NULL},
        (struct ListNode*)malloc(sizeof(struct ListNode)), *(lists[1]) = (struct ListNode){1, (struct ListNode*)malloc(sizeof(struct ListNode))}, *(lists[1]->next) = (struct ListNode){3, (struct ListNode*)malloc(sizeof(struct ListNode))}, *(lists[1]->next->next) = (struct ListNode){4, NULL},
        (struct ListNode*)malloc(sizeof(struct ListNode)), *(lists[2]) = (struct ListNode){2, (struct ListNode*)malloc(sizeof(struct ListNode))}, *(lists[2]->next) = (struct ListNode){6, NULL}
    };

    struct ListNode* result = mergeKLists(lists, 3);
    printList(result);

    return 0;
}
