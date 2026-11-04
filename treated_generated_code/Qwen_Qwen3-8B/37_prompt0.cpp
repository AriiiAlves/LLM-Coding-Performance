// ============================================================================
// [harness] Treated copy of generated_code/Qwen_Qwen3-8B/37_prompt0.cpp
// Changes (input/output + timing only):
//   1. LeetCode judge preamble below: all std headers + 'using namespace std'
//      (LeetCode adds these implicitly).
//   2. A main() after the END marker that reads the test, calls the function,
//      times the call and writes the output.
// The code between the BEGIN/END markers is byte-for-byte the AI-generated code.
// ============================================================================
#include <bits/stdc++.h>
using namespace std;
#define HH_CALL(fn) fn
// ===== BEGIN AI-GENERATED CODE (unchanged) =====
#include <vector>
using namespace std;

bool is_valid(int row, int col, char num, vector<vector<char>>& board) {
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == num) return false;
    }
    for (int i = 0; i < 9; i++) {
        if (board[i][col] == num) return false;
    }
    int start_row = row - row % 3;
    int start_col = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[start_row + i][start_col + j] == num) return false;
        }
    }
    return true;
}

bool find_empty(int& row, int& col, vector<vector<char>>& board) {
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (board[row][col] == '.') return true;
        }
    }
    return false;
}

bool backtrack(vector<vector<char>>& board) {
    int row, col;
    if (!find_empty(row, col, board)) return true;
    for (int num = 1; num <= 9; num++) {
        char num_char = '0' + num;
        if (is_valid(row, col, num_char, board)) {
            board[row][col] = num_char;
            if (backtrack(board)) return true;
            board[row][col] = '.';
        }
    }
    return false;
}

void solveSudoku(vector<vector<char>>& board) {
    backtrack(board);
}
// ===== END AI-GENERATED CODE =====

// ===================== [harness] I/O + timing (not AI-generated) =====================
// Usage: ./program <input file> <output file>
// - Reads every test case BEFORE timing starts (parsing is never timed).
// - Times only the calls to the solution function (steady_clock, summed over cases).
// - Peak extra memory: resets the kernel's peak-RSS counter after the input is
//   loaded, then reads it after all calls (so it is the solution's own memory).
// - Writes one result line per case, then "#STAT" lines with the measurements.
namespace hh {

struct Reader {
    std::vector<char> buf;
    size_t pos = 0;

    explicit Reader(const char* path) {
        FILE* f = std::fopen(path, "rb");
        if (!f) { std::perror("input"); std::exit(97); }
        std::fseek(f, 0, SEEK_END);
        long n = std::ftell(f);
        std::fseek(f, 0, SEEK_SET);
        buf.resize(n + 1);
        if (std::fread(buf.data(), 1, n, f) != (size_t)n) { std::perror("read"); std::exit(97); }
        buf[n] = 0;
        std::fclose(f);
    }
    void skip() { while (buf[pos] && std::isspace((unsigned char)buf[pos])) pos++; }
    long long num() {
        skip();
        bool neg = false;
        if (buf[pos] == '-') { neg = true; pos++; }
        long long v = 0;
        while (std::isdigit((unsigned char)buf[pos])) v = v * 10 + (buf[pos++] - '0');
        return neg ? -v : v;
    }
    std::string token() {
        skip();
        size_t s = pos;
        while (buf[pos] && !std::isspace((unsigned char)buf[pos])) pos++;
        return std::string(buf.data() + s, pos - s);
    }
    // "<length> <chars>", length 0 has no chars
    std::string str() {
        long long n = num();
        return n ? token() : std::string();
    }
    std::vector<int> ints() {
        long long n = num();
        std::vector<int> v(n);
        for (auto& x : v) x = (int)num();
        return v;
    }
};

static Reader* R = nullptr;
static FILE* OUT = nullptr;
static long long NS = 0;
static long BASE_KB = 0;
static std::chrono::steady_clock::time_point T0;

inline long status_kb(const char* key) {
    FILE* f = std::fopen("/proc/self/status", "r");
    if (!f) return 0;
    char line[256];
    long v = 0;
    size_t len = std::strlen(key);
    while (std::fgets(line, sizeof line, f))
        if (!std::strncmp(line, key, len)) { v = std::atol(line + len); break; }
    std::fclose(f);
    return v;
}

inline void init(int argc, char** argv) {
    if (argc < 3) { std::fprintf(stderr, "usage: %s <input> <output>\n", argv[0]); std::exit(98); }
    R = new Reader(argv[1]);
    OUT = std::fopen(argv[2], "w");
    if (!OUT) { std::perror("output"); std::exit(97); }
}

// Called after the whole input is loaded, right before the first timed call
inline void begin() {
    FILE* f = std::fopen("/proc/self/clear_refs", "w");  // "5" resets the peak RSS
    if (f) { std::fputs("5", f); std::fclose(f); }
    BASE_KB = status_kb("VmRSS:");
}

inline void tic() { T0 = std::chrono::steady_clock::now(); }
inline void toc() {
    NS += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - T0).count();
}
// Flushed per case, so a crash keeps the results of the cases before it
inline void flush() { std::fflush(OUT); }

inline void end() {
    long hwm = status_kb("VmHWM:");
    std::fprintf(OUT, "#STAT time_ns %lld\n#STAT mem_kb %ld\n", NS, std::max(0L, hwm - BASE_KB));
    std::fclose(OUT);
}

inline const char* boolstr(bool b) { return b ? "true" : "false"; }

}  // namespace hh

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
