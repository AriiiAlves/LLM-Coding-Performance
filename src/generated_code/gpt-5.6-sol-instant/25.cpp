class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0);
        dummy.next = head;

        ListNode* groupPrev = &dummy;

        while (true) {
            // Find the kth node.
            ListNode* kth = groupPrev;

            for (int i = 0; i < k && kth != nullptr; ++i)
                kth = kth->next;

            // Less than k nodes remain.
            if (kth == nullptr)
                break;

            ListNode* groupNext = kth->next;

            // Reverse current group.
            ListNode* prev = groupNext;
            ListNode* curr = groupPrev->next;

            while (curr != groupNext) {
                ListNode* next = curr->next;

                curr->next = prev;
                prev = curr;
                curr = next;
            }

            // Old first node becomes the last node.
            ListNode* oldStart = groupPrev->next;

            groupPrev->next = kth;
            groupPrev = oldStart;
        }

        return dummy.next;
    }
};