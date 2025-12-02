#include <string>
#include <vector>
#include <ranges>
#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <charconv>
#include <cctype>

using namespace std;

using Range  = pair<int64_t, int64_t>;
using Ranges = vector<Range>;

static int64_t to_i64(string_view s) {
    int64_t v{};
    auto [p, ec] = from_chars(s.data(), s.data() + s.size(), v);
    if (ec != errc{} || p != s.data() + s.size()) throw runtime_error("bad int");
    return v;
}

Ranges parse_ranges(string_view s) {
    Ranges out;
    while (!s.empty()) {
        auto comma = s.find(',');
        auto tok = s.substr(0, comma);
        s = (comma == string_view::npos) ? string_view{} : s.substr(comma + 1);

        auto dash = tok.find('-');
        if (dash == string_view::npos) throw runtime_error("missing '-'");
        out.emplace_back(to_i64(tok.substr(0, dash)), to_i64(tok.substr(dash + 1)));
    }
    return out;
}

Ranges get_test_data() {
    string test = R"(11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124)";
    Ranges ranges = parse_ranges(test);
    assert(ranges.size() == 11);
    return ranges;
}

Ranges get_data() {
    ifstream file("d02.txt");
    string s{istreambuf_iterator<char>(file), istreambuf_iterator<char>()};
    // trim
    while (!s.empty() && isspace(static_cast<unsigned char>(s.back())))
        s.pop_back();
    Ranges ranges = parse_ranges(s);
    assert(ranges.size() == 36);
    return ranges;
}

int64_t part1(Ranges& ranges) {
    int64_t sum = 0;
    for (auto [a, b] : ranges) {
        for (int64_t i = a; i <= b; i++) {
            string s = to_string(i);
            auto mid = s.size() / 2;
            string left  = s.substr(0, mid);
            string right = s.substr(mid);
            if (left == right) {
                sum += i;
            }
        }
    }
    return sum;
}

string repeat(string_view piece, size_t n) {
    string out;
    out.reserve(piece.size() * n);
    for (size_t i = 0; i < n; ++i) out += piece;   // or out.append(piece);
    return out;
}

int64_t part2(Ranges& ranges) {
    int64_t sum = 0;
    for (auto [a, b] : ranges) {
        for (int64_t i = a; i <= b; i++) {
            string s = to_string(i);
            auto mid = s.size() / 2;
            for (int j = 1; j <= mid; j++) {
                int n = s.size() / j;
                string t = repeat(s.substr(0, j), n);
                if (s == t) {
                    sum += i;
                    // cout << i << "\n";
                    break;
                }
            }
        }
    }
    return sum;
}

int main(int argc, char* argv[]) {
    Ranges ranges = (argc == 1) ? get_data() : get_test_data();
    cout << "Part1: " << part1(ranges) << "\n";
    cout << "Part2: " << part2(ranges) << "\n";
}
