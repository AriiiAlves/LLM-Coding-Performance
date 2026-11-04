// ============================================================================
// [harness] Treated copy of generated_code/Qwen_Qwen2.5-Coder-14B-Instruct/4_prompt0.cpp
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
#include <algorithm>
using namespace std;

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    int totalLength = nums1.size() + nums2.size();
    if (totalLength % 2 == 1) {
        return findKthElement(nums1, nums2, totalLength / 2 + 1);
    } else {
        return (findKthElement(nums1, nums2, totalLength / 2) + findKthElement(nums1, nums2, totalLength / 2 + 1)) / 2.0;
    }
}

int findKthElement(vector<int>& nums1, vector<int>& nums2, int k) {
    int index1 = 0, index2 = 0;
    int len1 = nums1.size(), len2 = nums2.size();
    
    while (true) {
        if (index1 == len1) {
            return nums2[index2 + k - 1];
        }
        if (index2 == len2) {
            return nums1[index1 + k - 1];
        }
        if (k == 1) {
            return min(nums1[index1], nums2[index2]);
        }
        
        int half = k / 2;
        int newIndex1 = min(index1 + half, len1) - 1;
        int newIndex2 = min(index2 + half, len2) - 1;
        
        if (nums1[newIndex1] < nums2[newIndex2]) {
            k -= (newIndex1 - index1 + 1);
            index1 = newIndex1 + 1;
        } else {
            k -= (newIndex2 - index2 + 1);
            index2 = newIndex2 + 1;
        }
    }
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
