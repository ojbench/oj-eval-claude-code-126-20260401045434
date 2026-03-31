/**
 * This is the student's part for Conway's Game of Life Simulator.
 * You need to finish all functions marked with "TODO" label.
 * When submitting, you should hand in THIS header file (i.e. game_of_life.h) to OJ to get correct mark.
 *
 * By the way, you SHOULD NOT try to read or write any extra information to the console.
 * Any attempt to hack the OJ or special judger WILL BE voided (and punished if it causes severe results).
 *
 * This is part of the 1st homework of Class A and 2nd homework of Class B. The workload isn't large.
 * We've also prepared a small visualizer for you. Enjoy coding ~
 */
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

/**
 * Feel free to define/modify variables here.
 */
inline int row = -1, col = -1;

// Use a set of coordinates to store live cells (more efficient for sparse grids)
inline std::unordered_set<long long> live_cells;

// Helper function to convert 2D coordinates to 1D key
inline long long coord_to_key(int x, int y) {
    return ((long long)x << 32) | (unsigned int)y;
}

// Helper function to extract x from key
inline int key_to_x(long long key) {
    return (int)(key >> 32);
}

// Helper function to extract y from key
inline int key_to_y(long long key) {
    return (int)(key & 0xFFFFFFFF);
}

/**
 * This function is called at the beginning of every game.
 * Read the initial map of the game from stdin.
 * The format of the input is written in the problem description.
 *
 * TODO: Read the initial map from input and initialize your variables.
 */
inline void Initialize() {
    std::cin >> col >> row;
    std::cin.ignore(); // Ignore the newline after row and col

    live_cells.clear();

    std::string pattern;
    std::string line;

    // Read all lines until we find '!'
    while (std::getline(std::cin, line)) {
        pattern += line;
        if (line.find('!') != std::string::npos) {
            break;
        }
    }

    // Parse the pattern
    int current_row = 0;
    int current_col = 0;
    int num = 0;
    bool has_num = false;

    for (size_t i = 0; i < pattern.length(); i++) {
        char c = pattern[i];

        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
            has_num = true;
        } else if (c == 'b') {
            int count = has_num ? num : 1;
            current_col += count;
            num = 0;
            has_num = false;
        } else if (c == 'o') {
            int count = has_num ? num : 1;
            for (int j = 0; j < count; j++) {
                if (current_row < row && current_col < col) {
                    live_cells.insert(coord_to_key(current_row, current_col));
                }
                current_col++;
            }
            num = 0;
            has_num = false;
        } else if (c == '$') {
            int count = has_num ? num : 1;
            current_row += count;
            current_col = 0;
            num = 0;
            has_num = false;
        } else if (c == '!') {
            break;
        }
    }
}

/**
 * This function is called once in every round of the game.
 * You need to simulate a round of game in this program.
 *
 * Note: You don't need to input or output anything. Just change the variables you defined so that they're in the new round.
 *
 * TODO: Simulate a new round of the game.
 */
inline void Tick() {
    std::unordered_set<long long> new_live_cells;
    std::unordered_map<long long, int> neighbor_count;

    // Count neighbors for all cells that might change
    for (long long key : live_cells) {
        int x = key_to_x(key);
        int y = key_to_y(key);

        // Increment neighbor count for all 8 neighbors
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;

                int nx = x + dx;
                int ny = y + dy;

                if (nx < 0 || nx >= row || ny < 0 || ny >= col) continue;

                long long nkey = coord_to_key(nx, ny);
                neighbor_count[nkey]++;
            }
        }
    }

    // Apply Conway's rules
    for (const auto& [key, count] : neighbor_count) {
        bool is_alive = live_cells.count(key) > 0;

        if (is_alive) {
            // Living cell with 2 or 3 neighbors survives
            if (count == 2 || count == 3) {
                new_live_cells.insert(key);
            }
        } else {
            // Dead cell with exactly 3 neighbors becomes alive
            if (count == 3) {
                new_live_cells.insert(key);
            }
        }
    }

    live_cells = std::move(new_live_cells);
}

/**
 * This function may be called at any time during the game.
 * You need to print the map of the game in a certain format.
 * Output format is written in the problem description.
 *
 * Note: In this function, you just needs to print the map. DO NOT simulate a new round of game.
 *
 * TODO: Print the whole map.
 */
inline void PrintGame() {
    std::cout << col << " " << row << "\n";

    if (live_cells.empty()) {
        std::cout << "!\n";
        return;
    }

    std::string result;

    // Build a map of rows that have live cells
    std::unordered_map<int, std::vector<int>> rows_with_cells;
    int max_row = -1;

    for (long long key : live_cells) {
        int r = key_to_x(key);
        int c = key_to_y(key);
        rows_with_cells[r].push_back(c);
        if (r > max_row) max_row = r;
    }

    // Sort columns in each row
    for (auto& [r, cols] : rows_with_cells) {
        std::sort(cols.begin(), cols.end());
    }

    int prev_row = -1;

    for (int r = 0; r <= max_row; r++) {
        if (rows_with_cells.find(r) == rows_with_cells.end()) {
            continue; // Empty row, will be handled by $ compression
        }

        // Add row separators for skipped rows
        if (r > prev_row + 1) {
            int skipped = r - prev_row;
            if (skipped > 1) {
                result += std::to_string(skipped) + "$";
            } else {
                result += "$";
            }
        } else if (prev_row >= 0) {
            result += "$";
        }
        prev_row = r;

        const auto& cols = rows_with_cells[r];
        int prev_col = -1;
        int consecutive_alive = 0;

        for (int c : cols) {
            // Check if this is consecutive with the previous live cell
            if (c == prev_col + 1) {
                consecutive_alive++;
            } else {
                // Flush previous consecutive alive cells
                if (consecutive_alive > 0) {
                    if (consecutive_alive > 1) {
                        result += std::to_string(consecutive_alive);
                    }
                    result += 'o';
                }

                // Add dead cells between live cells
                int dead_count = c - prev_col - 1;
                if (dead_count > 0) {
                    if (dead_count > 1) {
                        result += std::to_string(dead_count);
                    }
                    result += 'b';
                }

                consecutive_alive = 1;
            }
            prev_col = c;
        }

        // Flush remaining consecutive alive cells
        if (consecutive_alive > 0) {
            if (consecutive_alive > 1) {
                result += std::to_string(consecutive_alive);
            }
            result += 'o';
        }
    }

    result += '!';
    std::cout << result << "\n";
}

/**
 * This function may be called at any time during the game.
 * You need to return the number of live cells.
 *
 * @returns the number of live cells in the map.
 *
 * Note: In this function, you SHOULD NOT simulate a new round of game or print anything to stdout.
 *
 * TODO: Returns the number of live cells.
 */
inline int GetLiveCell() {
    return live_cells.size();
}
