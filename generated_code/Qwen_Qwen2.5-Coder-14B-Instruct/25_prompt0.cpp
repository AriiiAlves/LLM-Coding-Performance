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

    // Check if there are at least k nodes left
    ListNode* temp = head;
    for (int i = 0; i < k; ++i) {
        if (!temp) return head; // Less than k nodes, no need to reverse
        temp = temp->next;
    }

    // Reverse the first k nodes
    ListNode* prev = nullptr;
    ListNode* curr = head;
    ListNode* next = nullptr;
    for (int i = 0; i < k; ++i) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    // Recursively call for the next group
    if (next) {
        head->next = reverseKGroup(next, k);
    }

    return prev;
}
