#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ranges>
#include <iostream>
#include <cassert>
#include <charconv>

using namespace std;

using Row = vector<char>;
using Grid = vector<Row>;

Grid get_data(bool is_test) {
    string s;
    if (is_test) {
        s = R"(..@@.@@@@.
@@@.@.@.@@
@@@@@.@.@@
@.@@@@..@.
@@.@@@@.@@
.@@@@@@@.@
.@.@.@.@@@
@.@@@.@@@@
.@@@@@@@@.
@.@.@@@.@.
)";
    } else {
        ifstream file("d04.txt");
        s.assign(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
    }
    Grid grid;
    Row row;
    for (char c : s) {
        if (c == '\n') {
            if (!row.empty()) {
                row.push_back('.'); // padding
                grid.push_back(std::move(row));
                row.clear();
            }
        } else {
            if (row.size() == 0) // padding
                row.push_back('.');
            row.push_back(c);
        }
    }
    if (!row.empty()) grid.push_back(std::move(row)); // last line if no trailing \n
    // padding
    int w = grid[0].size();
    if (is_test)
        assert(w == 12);
    row.assign(w, '.');
    grid.insert(grid.begin(), row);
    grid.push_back(row);
    if (is_test)
        assert(grid.size() == 12);
    return grid;
}

int part1(Grid& grid) {
    int h = grid.size();
    int w = grid[0].size();
    int n_accessible = 0;
    for (int i = 1; i < h - 1; i++) {
        for (int j = 1; j < w - 1; j++) {
            // print("{}", grid[i][j]);
            if (grid[i][j] != '@') continue;
            int n = 0;
            for (int ii : {-1, 0, 1}) {
                for (int jj : {-1, 0, 1}) {
                    if (ii == 0 && jj == 0) continue;
                    n += (grid[i + ii][j + jj] == '@');
                }
            }
            n_accessible += n < 4 ? 1 : 0;
        }
        // println();
    }
    return n_accessible;
}

int remove_rolls(Grid& grid) {
    int h = grid.size();
    int w = grid[0].size();
    vector<pair<int, int>> removed;
    for (int i = 1; i < h - 1; i++) {
        for (int j = 1; j < w - 1; j++) {
            if (grid[i][j] != '@') continue;
            int n = 0;
            for (int ii : {-1, 0, 1}) {
                for (int jj : {-1, 0, 1}) {
                    if (ii == 0 && jj == 0) continue;
                    n += (grid[i + ii][j + jj] == '@');
                }
            }
            if (n < 4) {
                removed.emplace_back(i, j);
            }
        }
    }
    for (auto [i, j] : removed) {
        grid[i][j] = '.';
    }
    return removed.size();
}

int part2(Grid& grid) {
    int total_removed = 0;
    int n_removed = 0;
    while (n_removed > 0 || total_removed == 0) {
        n_removed = remove_rolls(grid);
        // println("Removed: {}", n_removed);
        total_removed += n_removed;
    }
    return total_removed;
}

int main(int argc, char* argv[]) {
    Grid grid = get_data(argc != 1);

    println("Grid: {} x {}", grid.size(), grid[0].size());
    println("Part1: {}", part1(grid));
    println("Part2: {}", part2(grid));
}