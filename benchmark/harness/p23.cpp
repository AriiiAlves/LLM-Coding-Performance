int main(int argc, char** argv) {
    hh::init(argc, argv);
    int T = (int)hh::R->num();
    std::vector<std::vector<ListNode*>> L(T);
    std::vector<long long> total(T, 0);
    for (int t = 0; t < T; t++) {
        int k = (int)hh::R->num();
        L[t].assign(k, nullptr);
        for (int i = 0; i < k; i++) {
            std::vector<int> v = hh::R->ints();
            total[t] += (long long)v.size();
            ListNode* head = nullptr;
            for (int j = (int)v.size() - 1; j >= 0; j--) { ListNode* n = new ListNode(v[j]); n->next = head; head = n; }
            L[t][i] = head;
        }
    }
    hh::begin();
    for (int t = 0; t < T; t++) {
        hh::tic();
        ListNode* r = HH_CALL(mergeKLists)(L[t]);
        hh::toc();
        // At most total+1 nodes: more means a cycle or a broken list
        long long cnt = 0;
        for (ListNode* p = r; p; p = p->next) {
            if (++cnt > total[t]) { std::fputs(" <CYCLE/EXTRA NODES>", hh::OUT); break; }
            std::fprintf(hh::OUT, cnt == 1 ? "%d" : " %d", p->val);
        }
        std::fputc('\n', hh::OUT);
        hh::flush();
    }
    hh::end();
}
