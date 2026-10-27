#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode* dummy = (ListNode*)malloc(sizeof(ListNode));
    ListNode* current = dummy;
    int carry = 0;

    while (l1 != NULL || l2 != NULL) {
        int sum = carry;
        if (l1 != NULL) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2 != NULL) {
            sum += l2->val;
            l2 = l2->next;
        }

        current->next = (ListNode*)malloc(sizeof(ListNode));
        current->next->val = sum % 10;
        carry = sum / 10;
        current = current->next;
    }

    if (carry > 0) {
        current->next = (ListNode*)malloc(sizeof(ListNode));
        current->next->val = carry;
    }

    ListNode* result = dummy->next;
    free(dummy);
    return result;
}

int main() {
    // Example 1
    ListNode* l1 = (ListNode*)malloc(sizeof(ListNode));
    l1->val = 2;
    l1->next = (ListNode*)malloc(sizeof(ListNode));
    l1->next->val = 4;
    l1->next->next = (ListNode*)malloc(sizeof(ListNode));
    l1->next->next->val = 3;

    ListNode* l2 = (ListNode*)malloc(sizeof(ListNode));
    l2->val = 5;
    l2->next = (ListNode*)malloc(sizeof(ListNode));
    l2->next->val = 6;
    l2->next->next = (ListNode*)malloc(sizeof(ListNode));
    l2->next->next->val = 4;

    ListNode* result = addTwoNumbers(l1, l2);

    while (result != NULL) {
        printf("%d ", result->val);
        result = result->next;
    }
    printf("\n");

    // Example 2
    ListNode* l1_2 = (ListNode*)malloc(sizeof(ListNode));
    l1_2->val = 0;

    ListNode* l2_2 = (ListNode*)malloc(sizeof(ListNode));
    l2_2->val = 0;

    result = addTwoNumbers(l1_2, l2_2);

    while (result != NULL) {
        printf("%d ", result->val);
        result = result->next;
    }
    printf("\n");

    // Example 3
    ListNode* l1_3 = (ListNode*)malloc(sizeof(ListNode));
    l1_3->val = 9;
    l1_3->next = (ListNode*)malloc(sizeof(ListNode));
    l1_3->next->val = 9;
    l1_3->next->next = (ListNode*)malloc(sizeof(ListNode));
    l1_3->next->next->val = 9;
    l1_3->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    l1_3->next->next->next->val = 9;
    l1_3->next->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    l1_3->next->next->next->next->val = 9;
    l1_3->next->next->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    l1_3->next->next->next->next->next->val = 9;
    l1_3->next->next->next->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    l1_3->next->next->next->next->next->next->val = 9;

    ListNode* l2_3 = (ListNode*)malloc(sizeof(ListNode));
    l2_3->val = 9;
    l2_3->next = (ListNode*)malloc(sizeof(ListNode));
    l2_3->next->val = 9;
    l2_3->next->next = (ListNode*)malloc(sizeof(ListNode));
    l2_3->next->next->val = 9;
    l2_3->next->next->next = (ListNode*)malloc(sizeof(ListNode));
    l2_3->next->next->next->val = 9;

    result = addTwoNumbers(l1_3, l2_3);

    while (result != NULL) {
        printf("%d ", result->val);
        result = result->next;
    }
    printf("\n");

    // Free all allocated memory
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

    while (l1_2 != NULL) {
        ListNode* temp = l1_2;
        l1_2 = l1_2->next;
        free(temp);
    }

    while (l2_2 != NULL) {
        ListNode* temp = l2_2;
        l2_2 = l2_2->next;
        free(temp);
    }

    while (l1_3 != NULL) {
        ListNode* temp = l1_3;
        l1_3 = l1_3->next;
        free(temp);
    }

    while (l2_3 != NULL) {
        ListNode* temp = l2_3;
        l2_3 = l2_3->next;
        free(temp);
    }

    while (result != NULL) {
        ListNode* temp = result;
        result = result->next;
        free(temp);
    }

    return 0;
}
