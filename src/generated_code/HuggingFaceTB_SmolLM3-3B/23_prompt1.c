#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

ListNode* mergeKLists(ListNode* lists[], int k) {
    if (k == 0) return NULL;

    ListNode dummy = {0, NULL};
    ListNode* current = &dummy;

    while (1) {
        int min_val = INT_MAX;
        ListNode* min_node = NULL;

        for (int i = 0; i < k; i++) {
            if (lists[i] != NULL && lists[i]->val < min_val) {
                min_val = lists[i]->val;
                min_node = lists[i];
            }
        }

        if (min_node == NULL) break;

        current->next = min_node;
        current = current->next;
        min_node->next = NULL;
        lists[min_node->val < min_node->next->val ? i : j] = min_node->next;
    }

    return dummy.next;
}

int main() {
    // Example 1
    ListNode* l1 = malloc(sizeof(ListNode));
    l1->val = 1;
    l1->next = malloc(sizeof(ListNode));
    l1->next->val = 4;
    l1->next->next = malloc(sizeof(ListNode));
    l1->next->next->val = 5;
    l1->next->next->next = NULL;

    ListNode* l2 = malloc(sizeof(ListNode));
    l2->val = 1;
    l2->next = malloc(sizeof(ListNode));
    l2->next->val = 3;
    l2->next->next = malloc(sizeof(ListNode));
    l2->next->next->val = 4;
    l2->next->next->next = NULL;

    ListNode* l3 = malloc(sizeof(ListNode));
    l3->val = 2;
    l3->next = malloc(sizeof(ListNode));
    l3->next->val = 6;
    l3->next->next = NULL;

    ListNode* lists[] = {l1, l2, l3};
    ListNode* result = mergeKLists(lists, 3);

    // Print the result
    while (result != NULL) {
        printf("%d ", result->val);
        result = result->next;
    }
    printf("\n");

    // Free the allocated memory
    while (l1 != NULL) {
        ListNode* temp = l1;
        l1 = l1->next;
        free(temp);
    }

    while (l2 != NULL) {
        ListNode* temp = l2;
        l2 = l2->next;
        free(temp);
    }

    while (l3 != NULL) {
        ListNode* temp = l3;
        l3 = l3->next;
        free(temp);
    }

    while (result != NULL) {
        ListNode* temp = result;
        result = result->next;
        free(temp);
    }

    return 0;
}
