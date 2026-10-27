#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode dummyHead;
    dummyHead.next = NULL;

    struct ListNode* p = &dummyHead;
    int carry = 0;

    while (l1 || l2 || carry) {
        int sum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + carry;
        carry = sum / 10;
        sum %= 10;

        p->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        p->next->val = sum;
        p->next->next = NULL;

        p = p->next;
        l1 = l1 ? l1->next : l1;
        l2 = l2 ? l2->next : l2;
    }

    return dummyHead.next;
}
