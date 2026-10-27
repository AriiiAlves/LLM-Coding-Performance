#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    struct ListNode dummy;
    dummy.next = head;
    struct ListNode *prevGroup = &dummy, *headGroup = head, *tailGroup = &dummy;
    while (1) {
        int count = 0;
        while (count < k && tailGroup != NULL) {
            tailGroup = tailGroup->next;
            count++;
        }
        if (count == k) {
            struct ListNode *nextGroup = prevGroup->next;
            prevGroup->next = head;
            head->next = tailGroup;
            prevGroup = head;
            head = nextGroup;
        } else {
            return dummy.next;
        }
    }
}
