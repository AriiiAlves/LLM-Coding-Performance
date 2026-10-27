struct ListNode* reverseKGroup(struct ListNode* head, int k) {
    if (!head || !head->next) return head;
    
    struct ListNode dummy = {0};
    dummy.next = head;
    struct ListNode *prev = &dummy, *cur = head, *next;
    int count = 0;
    
    while (cur) {
        next = cur->next;
        count++;
        
        if (count == k) {
            prev->next = reverse(prev->next, cur);
            prev = cur;
            count = 0;
        }
        
        cur = next;
    }
    
    return dummy.next;
}

// Helper function to reverse a sublist
struct ListNode* reverse(struct ListNode* start, struct ListNode* end) {
    struct ListNode *prev = NULL, *curr = start, *next;
    
    while (curr != end) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    
    return prev;
}
