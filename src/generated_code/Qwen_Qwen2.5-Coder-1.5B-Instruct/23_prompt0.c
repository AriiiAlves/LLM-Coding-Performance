#include <stdio.h>
#include <stdlib.h>

// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    if (listsSize == 0) return NULL;

    // Merge two sorted lists
    struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
        struct ListNode dummy(0);
        struct ListNode* curr = &dummy;

        while (l1 && l2) {
            if (l1->val < l2->val) {
                curr->next = l1;
                l1 = l1->next;
            } else {
                curr->next = l2;
                l2 = l2->next;
            }
            curr = curr->next;
        }

        curr->next = l1 ? l1 : l2;
        return dummy.next;
    }

    // Merge k sorted lists
    struct ListNode* mergeKListsHelper(struct ListNode** lists, int start, int end) {
        if (start >= end) return lists[start];

        int mid = start + (end - start) / 2;
        struct ListNode* left = mergeKListsHelper(lists, start, mid);
        struct ListNode* right = mergeKListsHelper(lists, mid + 1, end);

        return mergeTwoLists(left, right);
    }

    return mergeKListsHelper(lists, 0, listsSize - 1);
}

int main() {
    // Example usage
    struct ListNode* head1 = (struct ListNode*)malloc(sizeof(struct ListNode));
    head1->val = 1;
    head1->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head1->next->val = 4;
    head1->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head1->next->next->val = 5;

    struct ListNode* head2 = (struct ListNode*)malloc(sizeof(struct ListNode));
    head2->val = 1;
    head2->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head2->next->val = 3;
    head2->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head2->next->next->val = 4;

    struct ListNode* head3 = (struct ListNode*)malloc(sizeof(struct ListNode));
    head3->val = 2;
    head3->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head3->next->val = 6;

    struct ListNode** lists = {head1, head2, head3};
    struct ListNode* mergedList = mergeKLists(lists, 3);

    // Print the merged list
    while (mergedList) {
        printf("%d ", mergedList->val);
        mergedList = mergedList->next;
    }
    printf("\n");

    // Free allocated memory
    free(head1);
    free(head2);
    free(head3);
    free(mergedList);

    return 0;
}
