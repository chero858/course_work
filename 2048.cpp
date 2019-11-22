#include <random>
#include <iostream>
#include <numeric>
#include <string>
#include <cfloat>
#include <vector>

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
    std::uniform_real_distribution<double> dist(0, std::nextafter(max, DBL_MAX));
    return round(dist(mt));
}

class Game_2048{
private:
    vector<vector<int>> grid;
public:
    Game_2048() {
//        grid.resize(4);
//        for(int i = 0; i < 4; i++)
//            grid[i].resize(4);
        grid = {
                {2,0,0,0},
                {2,0,0,0},
                {2,0,0,0},
                {2,0,0,0}
        };
    }
    vector<vector<int>> make_move(const string& direction) {
        vector<vector<int>> board = grid;
        int dir_line = -1, dir_col = 0;
        int start_line = 0, start_column = 0, line_step = 1, column_step = 1;
        if(direction == "s") {
            start_line = 3;
            line_step = -1;
            dir_line = 1;
            dir_col = 0;
        }
        if(direction == "d") {
            start_column = 3;
            column_step = -1;
            dir_line = 0;
            dir_col = 1;
        }
        if(direction == "a") {
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
                        && (board[next_i][next_j] == board[i][j] || !board[next_i][next_j])) {
                        cout << i << ' ' << j << ' ' << board[i][j] << "   " << next_i << ' ' << next_j << " " << board[next_i][next_j] << endl;
                        board[next_i][next_j] += board[i][j];
                        board[i][j] = 0;
                        move_possible = true;
                    }
                }
            }
            // start_line++;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++)
                    cout << board[i][j] << "  ";
                cout << endl;
            }
            cout << endl;
            string foo;
            cin >> foo;
        } while(move_possible);
        return board;
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
        string input;
        while(input != "q" ){
            system("cls");
//            add_tile();
            show();
            cin >> input;
            grid = make_move(input);
            if(available_cells().empty()){
                if(make_move("w") == grid && make_move("a") == grid)
                    break;
            }
        }
        cout << 123;
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

int main() {
    Game_2048 game;
    game.play();
}