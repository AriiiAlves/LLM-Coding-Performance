#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode *dummyHead = malloc(sizeof(struct ListNode));
    struct ListNode *p = dummyHead;
    int carry = 0;

    while (l1 != NULL || l2 != NULL || carry) {
        int sum = carry;
        if (l1 != NULL) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2 != NULL) {
            sum += l2->val;
            l2 = l2->next;
        }
        carry = sum / 10;
        p->next = malloc(sizeof(struct ListNode));
        p->next->val = sum % 10;
        p = p->next;
    }
    struct ListNode *result = dummyHead->next;
    free(dummyHead);
    return result;
}

void printList(struct ListNode *head) {
    while (head != NULL) {
        printf("%d", head->val);
        head = head->next;
        if (head != NULL) printf(" ");
    }
    printf("\n");
}

int main() {
    struct ListNode *l1 = malloc(sizeof(struct ListNode));
    l1->val = 2;
    l1->next = malloc(sizeof(struct ListNode));
    l1->next->val = 4;
    l1->next->next = malloc(sizeof(struct ListNode));
    l1->next->next->val = 3;
    l1->next->next->next = NULL;

    struct ListNode *l2 = malloc(sizeof(struct ListNode));
    l2->val = 5;
    l2->next = malloc(sizeof(struct ListNode));
    l2->next->val = 6;
    l2->next->next = malloc(sizeof(struct ListNode));
    l2->next->next->val = 4;
    l2->next->next->next = NULL;

    struct ListNode *result = addTwoNumbers(l1, l2);
    printList(result);
    return 0;
}
