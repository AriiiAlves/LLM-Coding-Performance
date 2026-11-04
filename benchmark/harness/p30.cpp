int main(int argc, char** argv) {
    hh::init(argc, argv);
    int T = (int)hh::R->num();
    std::vector<std::string> S(T);
    std::vector<std::vector<std::string>> W(T);
    for (int t = 0; t < T; t++) {
        S[t] = hh::R->str();
        int k = (int)hh::R->num();
        W[t].resize(k);
        for (auto& w : W[t]) w = hh::R->str();
    }
    hh::begin();
    for (int t = 0; t < T; t++) {
        hh::tic();
        std::vector<int> r = HH_CALL(findSubstring)(S[t], W[t]);
        hh::toc();
        // Any order is accepted: the checker sorts the indices
        for (size_t i = 0; i < r.size(); i++) std::fprintf(hh::OUT, i ? " %d" : "%d", r[i]);
        std::fputc('\n', hh::OUT);
        hh::flush();
    }
    hh::end();
}
