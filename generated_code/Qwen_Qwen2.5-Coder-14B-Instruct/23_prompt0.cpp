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

// Comparator for the priority queue to sort nodes based on their values
struct Compare {
    bool operator()(ListNode* a, ListNode* b) {
        return a->val > b->val;
    }
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
    priority_queue<ListNode*, vector<ListNode*>, Compare> minHeap;
    
    // Push the head of each list into the min-heap
    for (ListNode* list : lists) {
        if (list != nullptr) {
            minHeap.push(list);
        }
    }
    
    ListNode dummyHead(0);
    ListNode* tail = &dummyHead;
    
    // Extract the smallest element from the heap and add it to the result list
    while (!minHeap.empty()) {
        ListNode* node = minHeap.top();
        minHeap.pop();
        
        tail->next = node;
        tail = tail->next;
        
        // If there is a next node in the list, push it into the heap
        if (node->next != nullptr) {
            minHeap.push(node->next);
        }
    }
    
    return dummyHead.next;
}
