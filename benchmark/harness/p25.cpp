int main(int argc, char** argv) {
    hh::init(argc, argv);
    int T = (int)hh::R->num();
    std::vector<ListNode*> H(T);
    std::vector<int> K(T), N(T);
    for (int t = 0; t < T; t++) {
        std::vector<int> v = hh::R->ints();
        K[t] = (int)hh::R->num();
        N[t] = (int)v.size();
        ListNode* head = nullptr;
        for (int j = (int)v.size() - 1; j >= 0; j--) { ListNode* n = new ListNode(v[j]); n->next = head; head = n; }
        H[t] = head;
    }
    hh::begin();
    for (int t = 0; t < T; t++) {
        hh::tic();
        ListNode* r = HH_CALL(reverseKGroup)(H[t], K[t]);
        hh::toc();
        long long cnt = 0;
        for (ListNode* p = r; p; p = p->next) {
            if (++cnt > N[t]) { std::fputs(" <CYCLE/EXTRA NODES>", hh::OUT); break; }
            std::fprintf(hh::OUT, cnt == 1 ? "%d" : " %d", p->val);
        }
        std::fputc('\n', hh::OUT);
        hh::flush();
    }
    hh::end();
}
