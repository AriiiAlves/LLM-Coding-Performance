#include <stdio.h>
#include <stdlib.h>

// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    struct ListNode dummy(0);
    dummy.next = head;
    struct ListNode *prev = &dummy, *curr = head, *next = NULL;
    int count = 0;

    while (curr) {
        count++;
        if (count % k == 0) {
            curr = prev->next;
            next = curr->next;
            curr->next = prev;
            prev = curr;
        } else {
            curr = curr->next;
        }
    }

    return dummy.next;
}

// Helper function to print the list
void printList(struct ListNode* head) {
    struct ListNode* curr = head;
    while (curr) {
        printf("%d ", curr->val);
        curr = curr->next;
    }
    printf("\n");
}

int main() {
    // Example usage
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = 1;
    head->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->next->val = 2;
    head->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->next->next->val = 3;
    head->next->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->next->next->next->val = 4;
    head->next->next->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->next->next->next->next->val = 5;

    int k = 2;
    struct ListNode* result = reverseKGroup(head, k);
    printList(result);

    return 0;
}
