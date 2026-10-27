#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

ListNode* mergeKLists(ListNode **lists, int k) {
    if (k == 0) return NULL;
    if (k == 1) return lists[0];

    ListNode *merged = NULL;
    ListNode *current = NULL;

    for (int i = 0; i < k; i++) {
        if (lists[i] != NULL) {
            merged = mergeTwoLists(merged, lists[i]);
        }
    }

    return merged;
}

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy;
    ListNode *tail = &dummy;
    dummy.next = NULL;

    while (l1 != NULL && l2 != NULL) {
        if (l1->val < l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }

    if (l1 != NULL) {
        tail->next = l1;
    } else {
        tail->next = l2;
    }

    return dummy.next;
}

void printList(ListNode *head) {
    while (head != NULL) {
        printf("%d", head->val);
        if (head->next != NULL) printf("->");
        head = head->next;
    }
    printf("\n");
}

int main() {
    ListNode *lists[3] = {NULL, NULL, NULL};
    // Example input: lists = [[1,4,5],[1,3,4],[2,6]]
    // Add your input here

    ListNode *result = mergeKLists(lists, 3);
    printList(result);

    return 0;
}
