#include <pybind11/pybind11.h>
#include <random>
#include <iostream>
#include <cfloat>
#include <vector>
#include <chrono>
#include <cmath>
#include <algorithm>

namespace py = pybind11;

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

//int random_int(int max) {
//    return std::rand() % (max + 1);
//}
//
//bool random_bool() {
//    return random_int(9) != 9;
//}

std::random_device rd;
std::mt19937 rand_engine(rd());

bool random_bool() {
    std::bernoulli_distribution random_bool_generator(0.9);
    return random_bool_generator(rand_engine);
}

int random_int(int max) {
    std::uniform_real_distribution<double> dist(0, std::nextafter(max + 1, DBL_MAX));
    return dist(rand_engine);
}

void print_grid(uint64_t num){
    vector<vector<int>> grid(4);
    long long int hex = 0xf;
    int shift = 0;
    for(int i = 3; i >= 0; i--)
        for(int j = 0; j  < 4; j++) {
            if(((num & hex) >> shift) != 0)
                grid[i].insert(grid[i].begin(), std::pow(2, (num & hex) >> shift));
            else grid[i].insert(grid[i].begin(), 0);
            hex *= 0xf + 1;
            shift += 4;
        }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            cout << grid[i][j] << "  ";
        cout << endl;
    }
    cout << endl;
}

bool check(uint16_t row, int index){
    return  ((row & (0xFULL << 4 * index)) == ((row & (0xFULL << 4 * (index + 1))) >> 4) && (row & (0xFULL << 4 * index)) != 0);
}

uint16_t sum_elements(uint16_t row){
    for(int i = 0; i < 3; i++) {
        if(check(row, i)) {
            if (i == 0)  {
                row++;
                row = ((row & 0xFF00ULL) >> 4) | (row & 0xFULL);
                continue;
            }
            if (i == 1) {
                row += 16;
                row = ((row & 0xF000ULL) >> 4) | (row & 0xFFULL);
            }
            else {
                row += 256;
                row &= 0x0FFFULL;
            }
        }
    }
    return row;
}

bool no_cells_available(uint64_t grid){
    for(int i = 0;  i < 16; i++){
        if ((grid & (0xFULL << (4 * i))) == 0)
            return false;
    }
    return true;
}

uint64_t insert_tile(uint64_t grid) {
    bool flag = true;
    if (no_cells_available(grid))
        return grid;
    while(flag){
        uint64_t position = random_int(15);
        if((grid & (0xFULL << (4 * position))) == 0){
            flag = false;
            uint64_t tile = 1;
            if(random_bool())
                tile <<= position * 4;
            else
                tile <<= position * 4 + 1;
            grid |= tile;
        }
    }
    return grid;
}



int score(uint64_t grid){
    int sum = 0;
    for(int i = 0;  i < 16; i++){
        sum += pow(2, ((grid & (0xFULL << (4 * i))) >> (4 * i)));
    }
    return sum;
}

uint64_t rotate_back(uint64_t grid) {
    uint64_t rotated_grid = (grid & 0xF000000000000ULL) << 12u;
    rotated_grid |= (grid & 0xF00000000ULL) << 24u;
    rotated_grid |= (grid & 0xF0000ULL) << 36u;
    rotated_grid |= (grid & 0xFULL) << 48u;

    rotated_grid |= (grid & 0xF0000000000000ULL) >> 8u;
    rotated_grid |= (grid & 0xF000000000ULL) << 4u;
    rotated_grid |= (grid & 0xF00000ULL) << 16u;
    rotated_grid |= (grid & 0xF0ULL) << 28u;

    rotated_grid |= (grid & 0xF00000000000000ULL) >> 28u;
    rotated_grid |= (grid & 0xF0000000000ULL) >> 16u;
    rotated_grid |= (grid & 0xF000000ULL) >> 4u;
    rotated_grid |= (grid & 0xF00ULL) << 8u;

    rotated_grid |= (grid & 0xF000000000000000ULL) >> 48u;
    rotated_grid |= (grid & 0xF00000000000ULL) >> 36u;
    rotated_grid |= (grid & 0xF0000000ULL) >> 24u;
    rotated_grid |= (grid & 0xF000ULL) >> 12u;

    return rotated_grid;
}


uint64_t rotate(uint64_t grid) {
    uint64_t rotated_grid = (grid & 0xF000ULL) << 48u;
    rotated_grid |= (grid & 0xF0000000ULL) << 28u;
    rotated_grid |= (grid & 0xF00000000000ULL) << 8u;
    rotated_grid |= (grid & 0xF000000000000000ULL) >> 12u;

    rotated_grid |= (grid & 0xF00ULL) << 36u;
    rotated_grid |= (grid & 0xF000000ULL) << 16u;
    rotated_grid |= (grid & 0xF0000000000ULL) >> 4u;
    rotated_grid |= (grid & 0xF00000000000000ULL) >> 24u;

    rotated_grid |= (grid & 0xF0ULL) << 24u;
    rotated_grid |= (grid & 0xF00000ULL) << 4u;
    rotated_grid |= (grid & 0xF000000000ULL) >> 16u;
    rotated_grid |= (grid & 0xF0000000000000ULL) >> 36u;

    rotated_grid |= (grid & 0xFULL) << 12u;
    rotated_grid |= (grid & 0xF0000ULL) >> 8u;
    rotated_grid |= (grid & 0xF00000000ULL) >> 28u;
    rotated_grid |= (grid & 0xF000000000000ULL) >> 48u;

    return rotated_grid;
}

uint64_t execute_turn_right(uint64_t grid){
    uint16_t row_mask = 0xFFFFULL;
    uint64_t executed_grid = 0;
    for(int i = 3; i >= 0; i--) {
        uint16_t row = (grid >> (i * 16)) & row_mask;
        if(row == 0)
            continue;
//    a b c d - start row
        while ((row & 0xFULL) == 0) // if d == 0
            row >>= 4; // 0 a b c
        if ((row & 0xFFF0ULL) != 0) {
            while ((row & 0xF0ULL) == 0) // if c == 0
                row = ((row & 0xFF00ULL) >> 4) | (row & 0xFULL); // 0 a b d
            if ((row & 0xF00ULL) == 0) // if b == 0
                row = ((row & 0xF000ULL) >> 4) | (row & 0xFFULL); // 0 a c d
        }
        row = sum_elements(row);
        uint64_t new_row = row; // create new var cause "row" only 16 bit
        executed_grid |= new_row << (16 * i);
    }
    return executed_grid;
}

uint64_t execute_turn_up(uint64_t grid) {
    return rotate_back(execute_turn_right(rotate(grid)));
}

uint64_t execute_turn_down(uint64_t grid) {
    return rotate(execute_turn_right(rotate_back(grid)));
}

uint64_t execute_turn_left(uint64_t grid) {
    return rotate(rotate(execute_turn_right(rotate(rotate(grid)))));
}

uint64_t execute_turn(uint64_t grid, int direction){
    switch(direction){
        case 0:
            return execute_turn_up(grid);
        case 1:
            return execute_turn_left(grid);
        case 2:
            return execute_turn_down(grid);
        case 3:
            return execute_turn_right(grid);
    }
}

int get_move(uint64_t grid) {
    int move = random_int(3);
    while(execute_turn(grid, move) == grid)
        move = random_int(3);
    return move;
}

bool is_game_over(uint64_t grid) {
    if(no_cells_available(grid))
        if (execute_turn(grid, 3) == grid && execute_turn(grid, 2) == grid)
            return true;
    return false;
}

int play(uint64_t &grid) {
    int move = 0;
    int dir = 0;
    bool flag = true;
    while(!is_game_over(grid)) {
        move = get_move(grid);
        if(flag){
            flag = false;
            dir = move;
        }
        grid = execute_turn(grid, move);
        grid = insert_tile(grid);
    }
    return dir;
}

int find_best_move(uint64_t grid) {
//    auto start = std::chrono::high_resolution_clock::now();
//    auto stop = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//
//    cout << "Time taken by function: "
//         << duration.count() << " microseconds" << endl;
//    std::srand(unsigned(time(nullptr)));
    uint64_t grid_copy = grid;
    vector<double> score_by_dir(4);
    vector<double> dirs(4);
    for(int i = 0; i < 10000; i++) {
        int dir = play(grid);
        score_by_dir[dir] += score(grid);
        dirs[dir]++;
        grid = grid_copy;
    }
//    print_grid(grid_copy);
    for(int i = 0; i < 4; i++)
        if(dirs[i] != 0)
            score_by_dir[i] /= dirs[i];
//    for(int i = 0; i < 4; i++)
//        cout << score_by_dir[i] << endl;
//    cout << "****************\n\n";
    int index = std::max_element(score_by_dir.begin(), score_by_dir.end()) - score_by_dir.begin();
    return index;
}
// best - 36236

PYBIND11_MODULE(example, m) {
m.doc() = "pybind11 example plugin"; // optional module docstring

m.def("find_best_move", &find_best_move, "A function which finds best move");

}