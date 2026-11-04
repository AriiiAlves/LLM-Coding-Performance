#include <vector>
#include <queue>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

struct Compare {
    bool operator()(ListNode* a, ListNode* b) {
        return a->val > b->val;
    }
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
    priority_queue<ListNode*, vector<ListNode*>, Compare> minHeap;
    for (auto list : lists) {
        while (list != nullptr) {
            minHeap.push(list);
            list = list->next;
        }
    }

    if (minHeap.empty()) {
        return nullptr;
    }

    ListNode* dummyHead = new ListNode();
    ListNode* current = dummyHead;

    while (!minHeap.empty()) {
        current->next = minHeap.top();
        minHeap.pop();
        current = current->next;
    }

    current->next = nullptr;
    return dummyHead->next;
}
