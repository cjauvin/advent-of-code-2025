#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <iostream>
#include <cassert>

int part1(std::vector<std::pair<int,int>>& values) {
    int dial = 50;
    int n_zeros = 0;

    for (const auto& [step, value] : values) {
        dial = (dial + step * value) % 100;
        if (dial < 0) dial += 100;
        n_zeros += (dial == 0);
    }

    return n_zeros;
}

int part2(std::vector<std::pair<int,int>>& values) {
    int dial = 50;
    int n_zeros = 0;

    for (const auto& [step, value] : values) {
        for (int i = 0; i < value; i++) {
            dial += step;
            dial = dial % 100;
            if (dial < 0) dial += 100;
            n_zeros += (dial == 0);
        }
    }

    return n_zeros;
}

// std::vector<std::string> get_test_data() {
//     std::string test = R"(L68
// L30
// R48
// L5
// R60
// L55
// L1
// L99
// R14
// L82
// )";
//     std::istringstream iss(test);
//     std::string line;
//     std::vector<std::string> lines;

//     while (std::getline(iss, line)) {
//         lines.push_back(std::move(line));
//     }

//     assert(lines.size() == 10);

//     return lines;
// }

std::vector<std::string> get_test_data() {
    constexpr std::string_view test = R"(L68
L30
R48
L5
R60
L55
L1
L99
R14
L82
)";
    std::vector<std::string> out;
    for (auto part : test | std::views::split('\n')) {
        if (!std::ranges::empty(part)) {
            out.emplace_back(part.begin(), part.end());  // make a std::string
        }
    }

    assert(out.size() == 10);

    return out;
}

std::vector<std::string> get_data() {
    std::ifstream file("d01.txt");
    std::vector<std::string> lines;

    for (std::string line : std::ranges::istream_view<std::string>(file)) {
        lines.push_back(std::move(line));
    }

    return lines;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> lines = (argc == 1) ? get_data() : get_test_data();
    std::vector<std::pair<int,int>> values;

    for (const auto& line : lines) {
        if (line.empty()) continue;
        // std::cout << line << "\n";
        int step = line[0] == 'R' ? 1 : -1;
        int value = std::stoi(line.substr(1));
        values.emplace_back(step, value);
    }

    std::cout << "Part1: " << part1(values) << "\n";
    std::cout << "Part2: " << part2(values) << "\n";
}