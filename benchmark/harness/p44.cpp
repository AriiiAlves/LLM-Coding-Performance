int main(int argc, char** argv) {
    hh::init(argc, argv);
    int T = (int)hh::R->num();
    std::vector<std::string> S(T), P(T);
    for (int t = 0; t < T; t++) { S[t] = hh::R->str(); P[t] = hh::R->str(); }
    hh::begin();
    for (int t = 0; t < T; t++) {
        hh::tic();
        bool r = HH_CALL(isMatch)(S[t], P[t]);
        hh::toc();
        std::fprintf(hh::OUT, "%s\n", hh::boolstr(r));
        hh::flush();
    }
    hh::end();
}
