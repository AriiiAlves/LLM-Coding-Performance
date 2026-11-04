int main(int argc, char** argv) {
    hh::init(argc, argv);
    int T = (int)hh::R->num();
    std::vector<std::string> S(T);
    for (int t = 0; t < T; t++) S[t] = hh::R->str();
    hh::begin();
    for (int t = 0; t < T; t++) {
        hh::tic();
        int r = HH_CALL(longestValidParentheses)(S[t]);
        hh::toc();
        std::fprintf(hh::OUT, "%d\n", r);
        hh::flush();
    }
    hh::end();
}
