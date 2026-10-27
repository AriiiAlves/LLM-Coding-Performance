#include <stdio.h>
#include <stdlib.h>

// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

// Helper function to merge two sorted linked lists
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    if (l1->val < l2->val) {
        l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    } else {
        l2->next = mergeTwoLists(l1, l2->next);
        return l2;
    }
}

// Function to merge k sorted linked lists
struct ListNode* mergeKLists(struct ListNode** lists, int k) {
    if (k <= 0) return NULL;

    // Use a priority queue to merge the lists
    struct ListNode* dummy = (struct ListNode*)malloc(sizeof(struct ListNode));
    dummy->next = NULL;
    struct ListNode* current = dummy;

    // Priority queue implementation using a min-heap
    struct ListNode** heap = (struct ListNode**)malloc(k * sizeof(struct ListNode*));
    for (int i = 0; i < k; i++) {
        heap[i] = lists[i];
    }

    while (1) {
        int minIndex = -1;
        for (int i = 0; i < k; i++) {
            if (heap[i]) {
                if (minIndex == -1 || heap[i]->val < heap[minIndex]->val) {
                    minIndex = i;
                }
            }
        }

        if (minIndex == -1) break;

        current->next = heap[minIndex];
        current = current->next;
        heap[minIndex] = heap[minIndex]->next;
    }

    struct ListNode* result = dummy->next;
    free(dummy);
    free(heap);
    return result;
}

// Example usage
int main() {
    // Create example input
    struct ListNode* l1 = (struct ListNode*)malloc(sizeof(struct ListNode));
    l1->val = 1;
    l1->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    l1->next->val = 4;
    l1->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    l1->next->next->val = 5;
    l1->next->next->next = NULL;

    struct ListNode* l2 = (struct ListNode*)malloc(sizeof(struct ListNode));
    l2->val = 1;
    l2->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    l2->next->val = 3;
    l2->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    l2->next->next->val = 4;
    l2->next->next->next = NULL;

    struct ListNode* l3 = (struct ListNode*)malloc(sizeof(struct ListNode));
    l3->val = 2;
    l3->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    l3->next->val = 6;
    l3->next->next = NULL;

    struct ListNode* lists[] = {l1, l2, l3};
    int k = 3;

    // Merge the lists
    struct ListNode* mergedList = mergeKLists(lists, k);

    // Print the merged list
    struct ListNode* temp = mergedList;
    while (temp) {
        printf("%d ", temp->val);
        temp = temp->next;
    }
    printf("\n");

    // Free memory
    while (mergedList) {
        struct ListNode* next = mergedList->next;
        free(mergedList);
        mergedList = next;
    }

    return 0;
}
