#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* mergeKLists(struct ListNode** lists, int k) {
    struct ListNode dummy;
    struct ListNode* curr = &dummy;
    int i;

    dummy.next = NULL;

    while(1) {
        int minIndex = -1;
        for(i = 0; i < k; i++) {
            if(lists[i] != NULL) {
                if(minIndex == -1 || lists[i]->val < lists[minIndex]->val) {
                    minIndex = i;
                }
            }
        }

        if(minIndex == -1) {
            break;
        }

        curr->next = lists[minIndex];
        curr = curr->next;
        lists[minIndex] = lists[minIndex]->next;
    }

    return dummy.next;
}

void printList(struct ListNode* head) {
    while(head != NULL) {
        printf("%d ", head->val);
        head = head->next;
    }
    printf("\n");
}

int main() {
    struct ListNode* lists[3];
    struct ListNode* head1 = (struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode* head2 = (struct ListNode*)malloc(sizeof(struct ListNode));
    struct ListNode* head3 = (struct ListNode*)malloc(sizeof(struct ListNode));

    head1->val = 1;
    head1->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head1->next->val = 4;
    head1->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head1->next->next->val = 5;
    head1->next->next->next = NULL;

    head2->val = 1;
    head2->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head2->next->val = 3;
    head2->next->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head2->next->next->val = 4;
    head2->next->next->next = NULL;

    head3->val = 2;
    head3->next = (struct ListNode*)malloc(sizeof(struct ListNode));
    head3->next->val = 6;
    head3->next->next = NULL;

    lists[0] = head1;
    lists[1] = head2;
    lists[2] = head3;

    struct ListNode* result = mergeKLists(lists, 3);
    printList(result);

    return 0;
}
