int main(int argc, char** argv) {
    hh::init(argc, argv);
    int T = (int)hh::R->num();
    std::vector<std::vector<int>> A(T), B(T);
    for (int t = 0; t < T; t++) { A[t] = hh::R->ints(); B[t] = hh::R->ints(); }
    hh::begin();
    for (int t = 0; t < T; t++) {
        hh::tic();
        double r = HH_CALL(findMedianSortedArrays)(A[t], B[t]);
        hh::toc();
        std::fprintf(hh::OUT, "%.5f\n", r);
        hh::flush();
    }
    hh::end();
}
