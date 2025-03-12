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
    vector<int> matrix;
    vector<Square> squares;

    Data(int n) : area(0), n(n), matrix(n, 0) {}

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
                for (int j = 0; j < n; ++j) {
                    cout << ((matrix[i] & (1 << (n - j - 1))) ? "1 " : "0 ");
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
        if (square.side >= n) {
            if (DEBUG) {
                cout << "Квадрат (" << square.x + 1 << ", " << square.y + 1 << ", " << square.side << ") не может быть размещен: выходит за пределы столешницы." << endl;
            }
            return false;
        }
        int s_row = get_row(n, square);
        bool can_place = (row | s_row) == (row ^ s_row);

        if (DEBUG) {
            cout << "Проверка квадрата: (" << square.x + 1 << ", " << square.y + 1 << ", " << square.side << ")" << endl;
            cout << "Результат проверки: " << (can_place ? "Можно разместить" : "Нельзя разместить") << endl;
            cout << "----------------------------------------" << endl;
        }

        return can_place;
    }

private:
    int get_row(int n, const Square& square) const {
        return (((1 << n) - 1) >> (n - square.side)) << (n - square.x - square.side);
    }
};

pair<int, int> get_info(int n) {
    if (n % 2 == 0) {
        return {n / 2, n / 2};
    }

    for (int d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            int d_max = n / d;
            int top_side = d_max * (d / 2 + 1);
            int low_side = n - top_side;
            return {top_side, low_side};
        }
    }

    int top_side = n / 2 + 1;
    int low_side = n - top_side;
    return {top_side, low_side};
}

void local_solve(Data& data, int& min_solve, Data& min_data) {
    if (data.area == data.n * data.n) {
        if (data.squares.size() < min_solve) {
            min_solve = data.squares.size();
            min_data = data;
        }
        return;
    }
    if (data.squares.size() >= min_solve) return;

    for (int y = 0; y < data.n; ++y) {
        for (int x = 0; x < data.n; ++x) {
            for (int side = data.n - max(x, y); side > 0; --side) {
                Square sq = {x, y, side};
                if (data.can_place(sq, data.matrix[y])) {
                    for (int s = side; s > 0; --s) {
                        Square new_sq = {x, y, s};
                        Data new_data = data;
                        int bin_mask = (((1 << data.n) - 1) >> (data.n - new_sq.side)) << (data.n - new_sq.x - new_sq.side);
                        new_data.add(new_sq, bin_mask);
                        local_solve(new_data, min_solve, min_data);
                    }
                    return;
                }
            }
        }
    }
}

int main() {
    int n;
    cin >> n;

    Data start(n);
    auto [top, low] = get_info(n);

    start.add({0, 0, top}, (((1 << n) - 1) >> (n - top)) << (n - top));
    start.add({0, top, low}, (((1 << n) - 1) >> (n - low)) << (n - low));
    start.add({top, 0, low}, (((1 << n) - 1) >> (n - low)) << (n - top - low));

    int min_solve = n * n;
    Data min_data(n);

    if (DEBUG) {
        cout << "Начальное состояние матрицы:" << endl;
        for (int i = 0; i < start.n; ++i) {
            for (int j = 0; j < start.n; ++j) {
                cout << ((start.matrix[i] & (1 << (start.n - j - 1))) ? "1 " : "0 ");
            }
            cout << endl;
        }
        cout << "Начальное количество квадратов: " << start.squares.size() << endl;
        cout << "Начальная площадь: " << start.area << endl;
        cout << "----------------------------------------" << endl;
    }

    local_solve(start, min_solve, min_data);

    cout << min_solve << endl;
    for (const auto& sq : min_data.squares) {
        cout << sq.x + 1 << " " << sq.y + 1 << " " << sq.side << endl;
    }

    return 0;
}