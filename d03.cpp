#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <iostream>
#include <cassert>
#include <charconv>

using namespace std;

vector<string> get_test_data() {
    string test = R"(987654321111111
811111111111119
234234234234278
818181911112111
)";
//    string test = R"(811111111111119)";
    vector<string> out;
    for (auto part : test | views::split('\n')) {
        if (!ranges::empty(part)) {
            out.emplace_back(part.begin(), part.end());  // make a std::string
        }
    }
    // assert(out.size() == 4);
    return out;
}

vector<string> get_data() {
    std::ifstream file("d03.txt");
    if (!file.is_open()) {
        throw std::runtime_error("Could not open input file: d03.txt");
    }
    std::vector<string> lines;
    for (string line : ranges::istream_view<string>(file)) {
        lines.push_back(std::move(line));
    }
    assert(lines.size() > 0);
    return lines;
}

int find_bigger_digit_pos(string_view s) {
    int max_d = 0;
    int max_i = -1;
    // for (auto [i, c] : views::enumerate(s)) {
    for (int i = 0; i < s.size(); ++i) {
        char c = s[i];
        int d = c - '0';
        if (d > max_d) {
            max_d = d;
            max_i = i;
        }
    }
    return max_i;
}

int64_t to_int64(const std::vector<char>& v) {
    string s(v.begin(), v.end());

    int64_t n{};
    auto [p, ec] = std::from_chars(s.data(), s.data() + s.size(), n);

    if (ec == std::errc::result_out_of_range)
        throw std::runtime_error("number out of int64 range");
    if (ec != std::errc{} || p != s.data() + s.size())
        throw std::runtime_error("bad number");

    return n;
}

// My butty one, without the correct window
int64_t find_largest_joltage_(string line, int n) {
    vector<char> digits;
    digits.reserve(n);
    int j = 0;
    for (int i = 0; i < n; i++) {
        string s = line.substr(j, line.size() - (n - i - 1));
        println("{}<--", s);
        int k = find_bigger_digit_pos(s);
        digits.push_back(line[j + k]);
        j += k + 1;
    }
    // println("{}", digits);
    return to_int64(digits);
}

// ChatGPT helped me to find the bug with the window calculation
int64_t find_largest_joltage(const std::string& line, int n) {
    std::string digits;
    digits.reserve(n);

    size_t j = 0;
    for (int i = 0; i < n; ++i) {
        size_t need_after = (size_t)(n - i - 1);
        size_t window_end_excl = line.size() - need_after;     // can pick indices < this
        size_t window_len = window_end_excl - j;               // MUST depend on j

        auto window = std::string_view(line).substr(j, window_len);
        int k = find_bigger_digit_pos(window);

        digits.push_back(window[(size_t)k]);
        j += (size_t)k + 1; // advance past chosen digit
    }

    int64_t out{};
    auto [p, ec] = std::from_chars(digits.data(), digits.data() + digits.size(), out);
    if (ec != std::errc{} || p != digits.data() + digits.size()) throw std::runtime_error("bad number");
    return out;
}

int64_t get_total_joltage(vector<string>& lines, int n) {
    int64_t sum = 0;
    for (auto line : lines) {
        int64_t j = find_largest_joltage(line, n);
        // println("{}", j);
        sum += j;
    }
    return sum;
}

int main(int argc, char* argv[]) {
    vector<string> lines = (argc == 1) ? get_data() : get_test_data();

    println("Part1: {}", get_total_joltage(lines, 2));
    println("Part2: {}", get_total_joltage(lines, 12));
}