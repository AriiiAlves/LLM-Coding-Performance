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
        if (l1) {
            carry += l1->val;
            l1 = l1->next;
        }
        if (l2) {
            carry += l2->val;
            l2 = l2->next;
        }

        p->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        p->next->val = carry % 10;
        carry /= 10;
        p = p->next;
    }

    return dummyHead.next;
}
