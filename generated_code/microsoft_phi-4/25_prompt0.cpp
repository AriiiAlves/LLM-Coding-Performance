#include <iostream>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode* reverseKGroup(ListNode* head, int k) {
    if (!head || k == 1) return head;

    ListNode* dummy = new ListNode(0);
    dummy->next = head;
    ListNode* prevGroupEnd = dummy;
    ListNode* curr = head;

    while (curr) {
        ListNode* kth = curr;
        for (int i = 1; i < k; ++i) {
            if (!kth->next) return dummy->next;
            kth = kth->next;
        }

        ListNode* nextGroupStart = kth->next;
        ListNode* prev = nextGroupStart;
        ListNode* curr = prevGroupEnd->next;

        while (curr != nextGroupStart) {
            ListNode* temp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = temp;
        }

        ListNode* temp = prevGroupEnd->next;
        prevGroupEnd->next = kth;
        prevGroupEnd = temp;
        prevGroupEnd->next = nextGroupStart;
    }

    return dummy->next;
}
