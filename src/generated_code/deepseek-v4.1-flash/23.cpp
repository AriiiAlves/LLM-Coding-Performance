class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
        for (auto node : lists)
            if (node) pq.push(node);
        ListNode dummy(0);
        ListNode* tail = &dummy;
        while (!pq.empty()) {
            ListNode* cur = pq.top(); pq.pop();
            tail->next = cur;
            tail = cur;
            if (cur->next) pq.push(cur->next);
        }
        return dummy.next;
    }
};