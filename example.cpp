#include <pybind11/pybind11.h>
#include <random>
#include <iostream>
#include <string>
#include <cfloat>
#include <utility>
#include <vector>
#include <chrono>

namespace py = pybind11;

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

bool random_bool() {
    std::random_device rd;
    std::mt19937 rand_engine(rd());
    std::bernoulli_distribution random_bool_generator(0.9);
    return random_bool_generator(rand_engine);
}

int random_int(int max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, std::nextafter(max + 1, DBL_MAX));
    return dist(mt);
}

class Game_2048{
private:
    vector<vector<int>> grid;
    int dir;
public:
    Game_2048() {
        grid.resize(4);
        for(int i = 0; i < 4; i++)
            grid[i].resize(4);
        dir = 0;
    }

    void set_grid(vector<vector<int>> board) {
        this->grid = std::move(board);
    }


    vector<vector<int>> make_move(const int& direction) {
        vector<vector<int>> board = grid;
        int dir_line = -1, dir_col = 0;
        int start_line = 0, start_column = 0, line_step = 1, column_step = 1;
        if(direction == 2) {
            start_line = 3;
            line_step = -1;
            dir_line = 1;
            dir_col = 0;
        }
        if(direction == 3) {
            start_column = 3;
            column_step = -1;
            dir_line = 0;
            dir_col = 1;
        }
        if(direction == 1) {
            dir_line = 0;
            dir_col = -1;
        }
        bool move_possible = true;
        do {
            move_possible = false;
            for(int i = start_line; i >= 0 && i < 4; i += line_step) {
                for (int j = start_column; j >= 0 && j < 4; j += column_step) {
                    int next_i = i + dir_line, next_j = j + dir_col;
                    if (board[i][j] && can_do_move(i, j, next_i, next_j)
                        && !board[next_i][next_j]) {
                        board[next_i][next_j] += board[i][j];
                        board[i][j] = 0;
                        move_possible = true;
                    }
                }
            }
        } while(move_possible);
        vector<vector<int>> board_copy;
        for(int k = 0; k < 2; k++) {
            if(k != 1)
                board_copy = board;
            for(int i = start_line; i >= 0 && i < 4; i += line_step) {
                for (int j = start_column; j >= 0 && j < 4; j += column_step) {
                    int next_i = i + dir_line, next_j = j + dir_col;
                    if (board[i][j] && can_do_move(i, j, next_i, next_j)
                        && (board[next_i][next_j] == board[i][j] || !board[next_i][next_j])
                        && (board_copy[i][j] == board[i][j])) {
                        board[next_i][next_j] += board[i][j];
                        board[i][j] = 0;
                    }
                }
            }
//            for (int i = 0; i < 4; i++) {
//                for (int j = 0; j < 4; j++)
//                    cout << board[i][j] << "  ";
//                cout << endl;
//            }
//            cout << endl;
//            string foo;
//            cin >> foo;
        }

        return board;
    }

    int Dir() {
        return this->dir;
    }

    static bool can_do_move(int line, int column, int next_line, int next_column ) {
        return (next_line >= 0 && next_column >= 0 && next_line < 4 && next_column < 4);
    }

    void add_tile() {
        bool tile = random_bool();
        int position = available_cells()[random_int(available_cells().size() - 1)];
        if(tile) grid[position / 4][position % 4] = 2;
        else grid[position / 4][position % 4] = 4;
    }

    vector<int> available_cells() {
        vector<int> available_cells;
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                if(grid[i][j] == 0)
                    available_cells.push_back(i * 4 + j);
        return available_cells;
    }

    void play() {
        int input = 0;
        bool flag = true;
        vector<vector<int>> grid_copy;
        while(input != 6 ){
//            system("cls");
            if (grid != grid_copy && available_cells().size())
                add_tile();
//            show();
            grid_copy = grid;
            input = random_int(3);
            if(flag) {
                flag = false;
                dir = input;
            }
            grid = make_move(input);
            if(available_cells().empty()){
                if(make_move(0) == grid && make_move(1) == grid)
                    break;
            }
        }
    }

    int score() {
        int sum = 0;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                sum += grid[i][j];
        return sum;
    }

    void show() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++)
                cout << grid[i][j] << "  ";
            cout << endl;
        }
        cout << score() << endl;
    }
};

int find_best_move(long long int num) {
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
    Game_2048 game;
//    auto start = std::chrono::high_resolution_clock::now();
//    auto stop = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//
//    cout << "Time taken by function: "
//         << duration.count() << " microseconds" << endl;
//    cout << game.score();
    vector<double> score_by_dir(4);
    for(int i = 0; i < 500; i++) {
        game.set_grid(grid);
        game.play();
        score_by_dir[game.Dir()] += game.score();
    }
    int max_score = score_by_dir[0];
    int index = 0;
    for(int i = 1; i < 4; i++)
        if(max_score < score_by_dir[i]) {
            max_score = score_by_dir[i];
            index = i;
        }
    return index;
}


PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("find_best_move", &find_best_move, "A function which finds best move");

}