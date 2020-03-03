#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void bin(uint64_t n) {
    if (n > 1)
        bin(n >> 1);
    printf("%d", n & 1);
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

uint64_t execute_turn_right(uint64_t grid){
    uint16_t row_mask = 0xFFFFULL;
    uint64_t executed_grid = 0;
    print_grid(grid);
    for(int i = 3; i >= 0; i--) {
        uint16_t row = (grid >> (i * 16)) & row_mask;
        if(row == 0)
            continue;
        int count_shifts = 0;
//    a b c d - start row
        while ((row & 0xFULL) == 0) { // if d == 0
            row >>= 4; // 0 a b c
            count_shifts++;
        }
        if ((row & 0xFFF0ULL) == 0) {
            goto skip;
        }
        while ((row & 0xF0ULL) == 0) // if c == 0
            row = ((row & 0xFF00ULL) >> 4) | (row & 0xFULL); // 0 a b d
        if ((row & 0xF00ULL) == 0) // if b == 0
            row = ((row & 0xF000ULL) >> 4) | (row & 0xFFULL); // 0 a c d
        skip:
        uint64_t new_row = row; // create new var cause "row" only 16 bit
        executed_grid |= new_row << (16 * i);
        print_grid(executed_grid);
    }
    print_grid(executed_grid);
    return executed_grid;
}


uint64_t rotate_back(uint64_t grid) {
    print_grid(grid);

    uint64_t rotated_grid = (grid & 0xF000000000000ULL) << 12;
    rotated_grid |= (grid & 0xF00000000ULL) << 24;
    rotated_grid |= (grid & 0xF0000ULL) << 36;
    rotated_grid |= (grid & 0xFULL) << 48;

    rotated_grid |= (grid & 0xF0000000000000ULL) >> 8;
    rotated_grid |= (grid & 0xF000000000ULL) << 4;
    rotated_grid |= (grid & 0xF00000ULL) << 16;
    rotated_grid |= (grid & 0xF0ULL) << 28;

    rotated_grid |= (grid & 0xF00000000000000ULL) >> 28;
    rotated_grid |= (grid & 0xF0000000000ULL) >> 16;
    rotated_grid |= (grid & 0xF000000ULL) >> 4;
    rotated_grid |= (grid & 0xF00ULL) << 8;

    rotated_grid |= (grid & 0xF000000000000000ULL) >> 48;
    rotated_grid |= (grid & 0xF00000000000ULL) >> 36;
    rotated_grid |= (grid & 0xF0000000ULL) >> 24;
    rotated_grid |= (grid & 0xF000ULL) >> 12;

    print_grid(rotated_grid);

    return rotated_grid;
}


uint64_t rotate(uint64_t grid) {
    print_grid(grid);

    uint64_t rotated_grid = (grid & 0xF000ULL) << 48;
    rotated_grid |= (grid & 0xF0000000ULL) << 28;
    rotated_grid |= (grid & 0xF00000000000ULL) << 8;
    rotated_grid |= (grid & 0xF000000000000000ULL) >> 12;

    rotated_grid |= (grid & 0xF00ULL) << 36;
    rotated_grid |= (grid & 0xF000000ULL) << 16;
    rotated_grid |= (grid & 0xF0000000000ULL) >> 4;
    rotated_grid |= (grid & 0xF00000000000000ULL) >> 24;

    rotated_grid |= (grid & 0xF0ULL) << 24;
    rotated_grid |= (grid & 0xF00000ULL) << 4;
    rotated_grid |= (grid & 0xF000000000ULL) >> 16;
    rotated_grid |= (grid & 0xF0000000000000ULL) >> 36;

    rotated_grid |= (grid & 0xFULL) << 12;
    rotated_grid |= (grid & 0xF0000ULL) >> 8;
    rotated_grid |= (grid & 0xF00000000ULL) >> 28;
    rotated_grid |= (grid & 0xF000000000000ULL) >> 48;

    print_grid(rotated_grid);

    return rotated_grid;
}

void check(uint64_t grid){
    uint16_t row_mask = 0xFFFFULL;
    print_grid(grid);
    bin(grid);
    cout << endl;
    uint16_t row = (grid >> 32) & row_mask; // сместить грид на 16*n (n - номер строки)
    uint16_t new_row = row ^ (row >> 4);
    vector<int> check_cells(3);
    uint16_t mask = 0xFULL;
    for(int i = 2; i >= 0; i--) {
        check_cells[i] = new_row & mask;
        mask <<= 4;
    }
    bin(check_cells[2]);
}


int main() {
    uint64_t num = 72357829431853072;
    execute_turn_right(num);
}