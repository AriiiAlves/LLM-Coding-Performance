#include <stdio.h>
#include <stdlib.h>

// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

// Helper function to reverse a sublist
struct ListNode* reverseSublist(struct ListNode* head, int k) {
    struct ListNode* prev = NULL;
    struct ListNode* curr = head;
    struct ListNode* next = NULL;

    for (int i = 0; i < k && curr != NULL; i++) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    return prev;
}

// Function to reverse nodes of the list k at a time
struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    if (head == NULL || k <= 1) {
        return head;
    }

    struct ListNode dummy;
    dummy.next = head;
    struct ListNode* prev = &dummy;
    struct ListNode* curr = head;

    while (curr != NULL) {
        struct ListNode* tail = curr;
        for (int i = 0; i < k && tail != NULL; i++) {
            tail = tail->next;
        }

        if (tail == NULL) {
            break;
        }

        struct ListNode* next = tail->next;
        tail->next = NULL;

        prev->next = reverseSublist(curr, k);
        curr->next = next;

        prev = curr;
        curr = next;
    }

    return dummy.next;
}

// Helper function to print the linked list
void printList(struct ListNode* head) {
    while (head != NULL) {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("\n");
}

int main() {
    // Example 1
    struct ListNode* head1 = malloc(sizeof(struct ListNode));
    head1->val = 1;
    head1->next = malloc(sizeof(struct ListNode));
    head1->next->val = 2;
    head1->next->next = malloc(sizeof(struct ListNode));
    head1->next->next->val = 3;
    head1->next->next->next = malloc(sizeof(struct ListNode));
    head1->next->next->next->val = 4;
    head1->next->next->next->next = malloc(sizeof(struct ListNode));
    head1->next->next->next->next->val = 5;
    head1->next->next->next->next->next = NULL;

    int k1 = 2;
    struct ListNode* result1 = reverseKGroup(head1, k1);
    printList(result1); // Output: 2 1 4 3 5

    // Example 2
    struct ListNode* head2 = malloc(sizeof(struct ListNode));
    head2->val = 1;
    head2->next = malloc(sizeof(struct ListNode));
    head2->next->val = 2;
    head2->next->next = malloc(sizeof(struct ListNode));
    head2->next->next->val = 3;
    head2->next->next->next = malloc(sizeof(struct ListNode));
    head2->next->next->next->val = 4;
    head2->next->next->next->next = malloc(sizeof(struct ListNode));
    head2->next->next->next->next->val = 5;
    head2->next->next->next->next->next = NULL;

    int k2 = 3;
    struct ListNode* result2 = reverseKGroup(head2, k2);
    printList(result2); // Output: 3 2 1 4 5

    // Free allocated memory
    struct ListNode* temp;
    while (head1 != NULL) {
        temp = head1;
        head1 = head1->next;
        free(temp);
    }
    while (head2 != NULL) {
        temp = head2;
        head2 = head2->next;
        free(temp);
    }

    return 0;
}
