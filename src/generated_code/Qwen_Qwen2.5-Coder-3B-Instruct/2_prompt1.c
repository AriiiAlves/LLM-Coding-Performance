#include <stdio.h>
#include <stdlib.h>

// Definition for singly-linked list.
struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode *head = NULL, *current = NULL;
    int carry = 0;

    while (l1 || l2 || carry) {
        int sum = carry;
        if (l1) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2) {
            sum += l2->val;
            l2 = l2->next;
        }
        carry = sum / 10;
        sum %= 10;

        if (!head) {
            head = current = (struct ListNode*)malloc(sizeof(struct ListNode));
            current->val = sum;
            current->next = NULL;
        } else {
            current->next = (struct ListNode*)malloc(sizeof(struct ListNode));
            current = current->next;
            current->val = sum;
            current->next = NULL;
        }
    }

    return head;
}

int main() {
    // Example 1
    struct ListNode *l1 = (struct ListNode*)malloc(sizeof(struct ListNode));
    l1->val = 2;
    l1->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    l1->next->val = 4;
    l1->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    l1->next->next->val = 3;
    l1->next->next->next = NULL;

    struct ListNode *l2 = (struct ListNode*)malloc(sizeof(struct ListNode));
    l2->val = 5;
    l2->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    l2->next->val = 6;
    l2->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    l2->next->next->val = 4;
    l2->next->next->next = NULL;

    struct ListNode *result = addTwoNumbers(l1, l2);

    // Print the result
    while (result) {
        printf("%d", result->val);
        result = result->next;
    }
    printf("\n");

    // Free allocated memory
    while (l1) {
        struct ListNode *temp = l1;
        l1 = l1->next;
        free(temp);
    }
    while (l2) {
        struct ListNode *temp = l2;
        l2 = l2->next;
        free(temp);
    }
    while (result) {
        struct ListNode *temp = result;
        result = result->next;
        free(temp);
    }

    return 0;
}
