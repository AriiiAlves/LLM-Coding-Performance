// Reference: iterative in-place reversal, O(n) time, O(1) memory
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode dummy(0, head);
    ListNode* gp = &dummy;
    while (true) {
        ListNode* kth = gp;
        for (int i = 0; i < k && kth; i++) kth = kth->next;
        if (!kth) break;
        ListNode* nxt = kth->next;
        ListNode* prev = nxt;
        ListNode* cur = gp->next;
        while (cur != nxt) { ListNode* t = cur->next; cur->next = prev; prev = cur; cur = t; }
        ListNode* first = gp->next;
        gp->next = kth;
        gp = first;
    }
    return dummy.next;
}
