#include <stdio.h>
#include <stdlib.h>

// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    if (!head || k <= 1) return head;

    struct ListNode dummy;
    dummy.next = head;
    struct ListNode *prev = &dummy, *cur = head, *next = NULL;
    int count = 0;

    while (cur) {
        count++;
        if (count % k == 0) {
            next = cur->next;
            prev->next = reverse(prev->next, cur);
            prev = cur;
            cur = next;
        } else {
            cur = cur->next;
        }
    }

    return dummy.next;
}

struct ListNode* reverse(struct ListNode* start, struct ListNode* end) {
    struct ListNode *prev = NULL, *cur = start, *next = NULL;
    while (cur != end) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

int main() {
    // Example usage:
    struct ListNode *head = malloc(sizeof(struct ListNode));
    head->val = 1;
    head->next = malloc(sizeof(struct ListNode));
    head->next->val = 2;
    head->next->next = malloc(sizeof(struct ListNode));
    head->next->next->val = 3;
    head->next->next->next = malloc(sizeof(struct ListNode));
    head->next->next->next->val = 4;
    head->next->next->next->next = malloc(sizeof(struct ListNode));
    head->next->next->next->next->val = 5;
    head->next->next->next->next->next = NULL;

    struct ListNode *result = reverseKGroup(head, 2);

    // Print the reversed list
    struct ListNode *temp = result;
    while (temp) {
        printf("%d ", temp->val);
        temp = temp->next;
    }
    printf("\n");

    // Free the allocated memory
    while (result) {
        struct ListNode *next = result->next;
        free(result);
        result = next;
    }

    return 0;
}
