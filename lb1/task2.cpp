#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

#define DEBUG 1

using namespace std;

struct Square {
    int x, y, side;
};

class Data {
public:
    int area;
    int n;
    int m;
    vector<int> matrix;
    vector<Square> squares;

    Data(int n, int m) : area(0), n(n), m(m), matrix(n, 0) {}

    Data add(const Square& square, int bin_mask) {
        for (int y = square.y; y < square.y + square.side; ++y) {
            matrix[y] |= bin_mask;
        }
        squares.push_back(square);
        area += square.side * square.side;

        if (DEBUG) {
            cout << "Добавлен квадрат: (" << square.x + 1 << ", " << square.y + 1 << ", " << square.side << ")" << endl;
            cout << "Текущее состояние матрицы:" << endl;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    cout << ((matrix[i] & (1 << (m - j - 1))) ? "1 " : "0 ");
                }
                cout << endl;
            }
            cout << "Текущее количество квадратов: " << squares.size() << endl;
            cout << "Текущая площадь: " << area << endl;
            cout << "----------------------------------------" << endl;
        }

        return *this;
    }

    bool can_place(const Square& square, int row) const {
        if (square.side >= n || square.side >= m) {
            if (DEBUG) {
                cout << "Квадрат (" << square.x + 1 << ", " << square.y + 1 << ", " << square.side << ") не может быть размещен: выходит за пределы столешницы." << endl;
            }
            return false;
        }
        int s_row = get_row(m, square);
        bool can_place = (row | s_row) == (row ^ s_row);

        if (DEBUG) {
            cout << "Проверка квадрата: (" << square.x + 1 << ", " << square.y + 1 << ", " << square.side << ")" << endl;
            cout << "Результат проверки: " << (can_place ? "Можно разместить" : "Нельзя разместить") << endl;
            cout << "----------------------------------------" << endl;
        }

        return can_place;
    }

private:
    int get_row(int m, const Square& square) const {
        return (((1 << m) - 1) >> (m - square.side)) << (m - square.x - square.side);
    }
};


void local_solve(Data& data, int& min_solve, Data& min_data, int& min_solve_count) {
    if (data.area == data.n * data.m) {
        min_solve_count++;
        if (data.squares.size() < min_solve) {
            min_solve = data.squares.size();
            min_data = data;
            min_solve_count = 1;
        }
        return;
    }
    if (data.squares.size() >= min_solve) return;

    for (int y = 0; y < data.n; ++y) {
        for (int x = 0; x < data.m; ++x) {
            for (int side = min(data.m - x, data.n - y); side > 0; --side) {
                Square sq = {x, y, side};
                if (data.can_place(sq, data.matrix[y])) {
                    for (int s = side; s > 0; --s) {
                        Square new_sq = {x, y, s};
                        Data new_data = data;
                        int bin_mask = (((1 << data.m) - 1) >> (data.m - new_sq.side)) << (data.m - new_sq.x - new_sq.side);
                        new_data.add(new_sq, bin_mask);
                        local_solve(new_data, min_solve, min_data, min_solve_count);
                    }
                    return;
                }
            }
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;

    Data start(n, m);

    int min_solve = n * m;
    int min_solve_count;
    Data min_data(n, m);

    if (DEBUG) {
        cout << "Начальное состояние матрицы:" << endl;
        for (int i = 0; i < start.n; ++i) {
            for (int j = 0; j < start.m; ++j) {
                cout << ((start.matrix[i] & (1 << (start.m - j - 1))) ? "1 " : "0 ");
            }
            cout << endl;
        }
        cout << "Начальное количество квадратов: " << start.squares.size() << endl;
        cout << "Начальная площадь: " << start.area << endl;
        cout << "----------------------------------------" << endl;
    }

    local_solve(start, min_solve, min_data, min_solve_count);

    cout << min_solve << endl;
    for (const auto& sq : min_data.squares) {
        cout << sq.x + 1 << " " << sq.y + 1 << " " << sq.side << endl;
    }
    cout << "Количество минимальных решений: " << min_solve_count << endl;

    return 0;
}