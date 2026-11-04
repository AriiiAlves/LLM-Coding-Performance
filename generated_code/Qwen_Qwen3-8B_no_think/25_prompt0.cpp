#include <vector>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode* reverseKGroup(ListNode* head, int k) {
    if (!head || k <= 0) return head;

    // Helper function to reverse a sublist from start to end
    ListNode* reverse(ListNode* start, ListNode* end) {
        ListNode* prev = nullptr;
        ListNode* curr = start;
        ListNode* next = nullptr;
        while (curr != end) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        curr->next = prev;
        return curr;
    }

    // Find the length of the linked list
    int length = 0;
    ListNode* temp = head;
    while (temp) {
        length++;
        temp = temp->next;
    }

    // Create dummy node to handle edge cases
    ListNode dummy(0, head);
    ListNode* prevGroupEnd = &dummy;
    ListNode* current = head;

    for (int i = 0; i < length / k; ++i) {
        // Find the end of the current group
        ListNode* end = current;
        for (int j = 0; j < k - 1; ++j) {
            end = end->next;
        }

        // Reverse the current group
        ListNode* newStart = reverse(current, end);
        prevGroupEnd->next = newStart;
        prevGroupEnd = current;

        // Move to the next group
        current = prevGroupEnd->next;
    }

    return dummy.next;
}
