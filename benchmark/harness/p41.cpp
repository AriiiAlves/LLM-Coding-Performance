int main(int argc, char** argv) {
    hh::init(argc, argv);
    int T = (int)hh::R->num();
    std::vector<std::vector<int>> V(T);
    for (int t = 0; t < T; t++) V[t] = hh::R->ints();
    hh::begin();
    for (int t = 0; t < T; t++) {
        hh::tic();
        int r = HH_CALL(firstMissingPositive)(V[t]);
        hh::toc();
        std::fprintf(hh::OUT, "%d\n", r);
        hh::flush();
    }
    hh::end();
}
