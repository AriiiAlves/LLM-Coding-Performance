int main(int argc, char** argv) {
    hh::init(argc, argv);
    int T = (int)hh::R->num();
    std::vector<std::vector<std::vector<char>>> B(T, std::vector<std::vector<char>>(9, std::vector<char>(9)));
    for (int t = 0; t < T; t++) {
        std::string s = hh::R->token();
        for (int i = 0; i < 81; i++) B[t][i / 9][i % 9] = s[i];
    }
    hh::begin();
    for (int t = 0; t < T; t++) {
        hh::tic();
        HH_CALL(solveSudoku)(B[t]);
        hh::toc();
        for (int i = 0; i < 81; i++) std::fputc(B[t][i / 9][i % 9], hh::OUT);
        std::fputc('\n', hh::OUT);
        hh::flush();
    }
    hh::end();
}
