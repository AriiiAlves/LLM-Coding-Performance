#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode* dummy = (ListNode*)malloc(sizeof(ListNode));
    dummy->next = head;
    ListNode* prev = dummy;
    ListNode* current = head;
    while (1) {
        ListNode* next = current;
        int count = 0;
        while (count < k && next) {
            next = next->next;
            count++;
        }
        if (count < k) break;
        ListNode* groupStart = current;
        for (int i = 0; i < k - 1; i++) {
            ListNode* nextNode = current->next;
            current->next = prev->next;
            prev->next = current;
            current = nextNode;
        }
        prev = groupStart;
    }
    ListNode* result = dummy->next;
    free(dummy);
    return result;
}

int main() {
    // Example 1
    ListNode* head1 = (ListNode*)malloc(sizeof(ListNode));
    head1->val = 1;
    head1->next = (ListNode*)malloc(sizeof(ListNode));
    head1->next->val = 2;
    head1->next->next = (ListNode*)malloc(sizeof(ListNode));
    head1->next->next->val = 3;
    head1->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    head1->next->next->next->val = 4;
    head1->next->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    head1->next->next->next->next->val = 5;
    head1->next->next->next->next->next = NULL;
    int k1 = 2;
    ListNode* result1 = reverseKGroup(head1, k1);
    while (result1) {
        printf("%d ", result1->val);
        result1 = result1->next;
    }
    printf("\n");

    // Example 2
    ListNode* head2 = (ListNode*)malloc(sizeof(ListNode));
    head2->val = 1;
    head2->next = (ListNode*)malloc(sizeof(ListNode));
    head2->next->val = 2;
    head2->next->next = (ListNode*)malloc(sizeof(ListNode));
    head2->next->next->val = 3;
    head2->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    head2->next->next->next->val = 4;
    head2->next->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    head2->next->next->next->next->val = 5;
    head2->next->next->next->next->next = NULL;
    int k2 = 3;
    ListNode* result2 = reverseKGroup(head2, k2);
    while (result2) {
        printf("%d ", result2->val);
        result2 = result2->next;
    }
    printf("\n");

    // Free memory
    while (head1) {
        ListNode* temp = head1;
        head1 = head1->next;
        free(temp);
    }
    while (head2) {
        ListNode* temp = head2;
        head2 = head2->next;
        free(temp);
    }
    return 0;
}
