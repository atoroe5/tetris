#include <iostream>
#include <conio.h>
#include <windows.h>
#include <limits>
using namespace std;

/* Пример анимации для игры тетрис на ОС Windows */

const int WIDTH = 10;   // Константные значения (работают, как переменные, ...
const int HEIGTH = 20;  //                       ... но их нельзя изменять)
int a = -1;
int l = 1, will;
int item_x, item_y;     // Координаты падающего блока
int item_color = 1;     // Цвет блока (может быть от 1 до 7)
int item_type = 1;
bool item[4][4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // Этот массив будет отличаться для каждого варианта
int records[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int points = 0;
int item_size_x = 2, item_size_y = 2;
int field[HEIGTH][WIDTH] = { 0 };  // Поле тетриса

HANDLE h; // Системная переменная-идентификатор окна.

// Функция отрисовки блока на поле.
// Используется при движении блока по полю
void insert_item_to_field(int x, int y, bool item[4][4], int color) {
    for (int i = item_size_y - 1; i >= 0; --i)
        for (int j = 0; j < item_size_x; ++j)
            if (item[i][j])
                field[y - i][x + j] = color;
}

void remove_item_from_field(int x, int y, bool item[4][4]) {
    for (int i = item_size_y - 1; i >= 0; --i)
        for (int j = 0; j < item_size_x; ++j)
            if (item[i][j])
                field[y - i][x + j] = 0;
}


void generate_new_item() {
    item_x = WIDTH / 2 - item_size_x / 2;     // Точка равноудалена от боков поля
    item_y = -1 + item_size_y;             // 0 - самый верх поля
    insert_item_to_field(item_x, item_y, item, item_color);
}

void draw_point(int point) {
    switch (point) {
    case 0: // Отрисовка пустого места
        cout << "."; break;
    case 1:
        // Отрисовка нашего блока с другим цветом
        SetConsoleTextAttribute(h, 0x0001);
        cout << "X";
        SetConsoleTextAttribute(h, 0x0008);
        break;
    case 2:
        SetConsoleTextAttribute(h, 0x0002);
        cout << "X";
        SetConsoleTextAttribute(h, 0x0008);
        break;
    case 3:
        SetConsoleTextAttribute(h, 0x0003);
        cout << "X";
        SetConsoleTextAttribute(h, 0x0008);
        break;
    case 4:
        SetConsoleTextAttribute(h, 0x0004);
        cout << "X";
        SetConsoleTextAttribute(h, 0x0008);
        break;
    case 5:
        SetConsoleTextAttribute(h, 0x0005);
        cout << "X";
        SetConsoleTextAttribute(h, 0x0008);
        break;
    case 6:
        SetConsoleTextAttribute(h, 0x0006);
        cout << "X";
        SetConsoleTextAttribute(h, 0x0008);
        break;
    case 7:
        SetConsoleTextAttribute(h, 0x0007);
        cout << "X";
        SetConsoleTextAttribute(h, 0x0008);
        break;
    }
    cout << " ";
}

void draw_field() {
    cout << "+";
    for (int i = 0; i < HEIGTH; ++i)
        cout << "-";
    cout << "+   ";
    SetConsoleTextAttribute(h, 0x0001);
    cout << "points: " << points << "\r\n";
    SetConsoleTextAttribute(h, 0x0008);
    for (int i = 0; i < HEIGTH; ++i) {
        cout << "|";
        for (int j = 0; j < WIDTH; ++j)
            draw_point(field[i][j]);
        cout << "|";
        cout << "\r\n";   // Нужно писать так из-за ncurses
    }
    cout << "+";
    for (int i = 0; i < HEIGTH; ++i)
        cout << "-";
    cout << "+\r\n";
}

void process_fall(int* y) { // Здесь мы реализуем падение блока вниз
    (*y)++;
}

bool check_exit(int x, int y) {
    if (item_y >= HEIGTH - 1) {
        return true;
    } else {
        int p = 0;
        if (item_type == 1 || item_type == 2 || item_type == 3 || item_type == 6 || item_type == 7 || item_type == 9 || item_type == 10 || item_type == 16) {
            for (int i = 0; i < item_size_x; i++) {
                if (field[item_y + 1][item_x + i] != 0) {
                    p = 1;
                    break;
                }
            }
        }
        else if (item_type == 4) {
            if (field[item_y + 1][item_x] != 0) {
                p = 1;
            }
            if (field[item_y - 1][item_x + 1] != 0) {
                p = 1;
            }
        }
        else if (item_type == 5) {
            if (field[item_y][item_x] != 0) {
                p = 1;
            }
            if (field[item_y][item_x + 1] != 0) {
                p = 1;
            }
            if (field[item_y + 1][item_x + 2] != 0) {
                p = 1;
            }
        }
        else if (item_type == 8) {
            if (field[item_y - 1][item_x] != 0) {
                p = 1;
            }
            if (field[item_y + 1][item_x + 1] != 0) {
                p = 1;
            }
        }
        else if (item_type == 11) {
            if (field[item_y + 1][item_x] != 0) {
                p = 1;
            }
            if (field[item_y][item_x + 1] != 0) {
                p = 1;
            }
            if (field[item_y][item_x + 2] != 0) {
                p = 1;
            }
        }
        else if (item_type == 13) {
            if (field[item_y + 1][item_x] != 0) {
                p = 1;
            }
            if (field[item_y][item_x + 1] != 0) {
                p = 1;
            }
        }
        else if (item_type == 12) {
            if (field[item_y + 1][item_x + 2] != 0) {
                p = 1;
            }
            if (field[item_y + 1][item_x + 1] != 0) {
                p = 1;
            }
            if (field[item_y][item_x] != 0) {
                p = 1;
            }
        }
        else if (item_type == 15) {
            if (field[item_y][item_x] != 0) {
                p = 1;
            }
            if (field[item_y + 1][item_x + 1] != 0) {
                p = 1;
            }
        }
        else if (item_type == 14) {
            if (field[item_y][item_x + 2] != 0) {
                p = 1;
            }
            if (field[item_y + 1][item_x + 1] != 0) {
                p = 1;
            }
            if (field[item_y + 1][item_x] != 0) {
                p = 1;
            }
        }
        else if (item_type == 19) {
            if (field[item_y][item_x] != 0) {
                p = 1;
            }
            if (field[item_y + 1][item_x + 1] != 0) {
                p = 1;
            }
        }
        else if (item_type == 18) {
            if (field[item_y][item_x + 2] != 0) {
                p = 1;
            }
            if (field[item_y + 1][item_x + 1] != 0) {
                p = 1;
            }
            if (field[item_y][item_x] != 0) {
                p = 1;
            }
        }
        else if (item_type == 17) {
            if (field[item_y + 1][item_x] != 0) {
                p = 1;
            }
            if (field[item_y][item_x + 1] != 0) {
                p = 1;
            }
        }
        if (p == 1) {
            return true;
        } else {
            return false;
        }
    }
}
void check_field() {
    int p = 0, kol = 0;
    for (int i = 0; i < HEIGTH; i++) {
        p = 0;
        for (int u = 0; u < WIDTH; u++) {
            if (field[i][u] == 0) {
                p = 1;
            }
        }
        if (p == 0) {
            kol += 1;
            for (int y = 0; y < WIDTH; y++) {
                field[i][y] = 0;
            }
            system("CLS");
            draw_field();
            Sleep(100);
            for (int y = 0; y < WIDTH; y++) {
                field[i][y] = 1;
            }
            Sleep(100);
            system("CLS");
            draw_field();
            for (int y = 0; y < WIDTH; y++) {
                field[i][y] = 0;
            }
            system("CLS");
            draw_field();
            Sleep(100);
            for (int a = i - 1; a >= 0; a--) {
                for (int j = 0; j < WIDTH; j++) {
                    if (field[a][j] != 0) {
                        field[a + 1][j] = field[a][j];
                        field[a][j] = 0;
                    }
                }
            }
            system("CLS");
            draw_field();
        }
    }
    if (kol != 0) {
        switch (kol) {
        case 1:
            kol = 100;
            break;
        case 2:
            kol = 300;
            break;
        case 3:
            kol = 700;
            break;
        case 4:
            kol = 1500;
            break;
        }
        points += kol;
        system("CLS");
        draw_field();
        Sleep(80);
        points -= kol;
        system("CLS");
        draw_field();
        Sleep(80);
        points += kol;
        system("CLS");
        draw_field();
    }
}
void process_keyboard() {
    char c = 0;
    if (_kbhit()) c = _getch();
    if (l != 5) {
        if (c == 'a' && item_x > 0) {
            int p = 0;
            if (item_type == 1 || item_type == 2 || item_type == 3 || item_type == 4 || item_type == 7 || item_type == 10 || item_type == 11 || item_type == 17) {
                for (int i = 0; i < item_size_y; i++) {
                    if (field[item_y - i][item_x - 1] != 0) {
                        p = 1;
                        break;
                    }
                }
            }
            else if (item_type == 5) {
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 6) {
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 9) {
                if (field[item_y - 1][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 8) {
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 12) {
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 13) {
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 14) {
                if (field[item_y - 1][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 15) {
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 16) {
                if (field[item_y - 1][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 18) {
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 19) {
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x] != 0) {
                    p = 1;
                }
            }
            if (p == 0)
                item_x--;
        }
        if (c == 'd' && item_x < WIDTH - item_size_x) {
            int p = 0;
            if (item_type == 1 || item_type == 2 || item_type == 3 || item_type == 5 || item_type == 6 || item_type == 8 || item_type == 9 || item_type == 19) {
                for (int i = 0; i < item_size_y; i++) {
                    if (field[item_y - i][item_x + item_size_x] != 0) {
                        p = 1;
                        break;
                    }
                }
            }
            else if (item_type == 7) {
                if (field[item_y][item_x + 3] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 4) {
                if (field[item_y][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 2] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 11) {
                if (field[item_y][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 3] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 10) {
                if (field[item_y][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 12) {
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x+3] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 13) {
                if (field[item_y][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 2] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 14) {
                if (field[item_y - 1][item_x + 3] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x + 2] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 15) {
                if (field[item_y][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 16) {
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x + 3] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 18) {
                if (field[item_y - 1][item_x + 3] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x + 2] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 17) {
                if (field[item_y][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 1] != 0) {
                    p = 1;
                }
            }
            if (p == 0)
                item_x++;
        }
        if (c == 'w' && item_type != 1) {
            if (item_type == 3) {
                if (field[item_y][item_x + 1] != 0 || field[item_y][item_x + 2] != 0 || field[item_y][item_x + 3] != 0 || item_x + 3 > WIDTH) {
                    if (field[item_y][item_x - 1] != 0 || item_x - 1 < 0) {
                        item_type = 3;
                    }
                    else {
                        item_x--;
                        if (field[item_y][item_x + 2] != 0 || field[item_y][item_x + 3] != 0 || item_x + 3 > WIDTH) {
                            if (field[item_y][item_x - 1] != 0 || item_x - 1 < 0) {
                                item_type = 3;
                                item_x++;
                            }
                            else {
                                item_x--;
                                if (field[item_y][item_x + 3] != 0 || item_x + 3 >= WIDTH) {
                                    if (field[item_y][item_x - 1] != 0 || item_x - 1 < 0) {
                                        item_type = 3;
                                        item_x += 2;
                                    }
                                    else {
                                        item_x--;
                                        item_type = 2;
                                    }
                                }
                                else {
                                    item_type = 2;
                                }
                            }
                        }
                    }
                }
                else {
                    item_type = 2;
                }
            }
            else if (item_type == 2 && field[item_y+1][item_x] == 0 && field[item_y+2][item_x] == 0 && field[item_y+3][item_x] == 0) {
                item_type = 3;
            }
            else if (item_type == 4 && field[item_y + 1][item_x+2] == 0 && field[item_y][item_x + 2] == 0) {
                item_type = 5;
            }
            else if (item_type == 5) {
                item_type = 6;
            }
            else if (item_type == 6) {
                item_type = 7;
            }
            else if (item_type == 7) {
                item_type = 4;
            }
            else if (item_type == 8 && field[item_y][item_x] == 0 && field[item_y + 1][item_x+2] == 0 && field[item_y][item_x+2] == 0) {
                item_type = 9;
            }
            else if (item_type == 9 && field[item_y + 1][item_x] == 0) {
                if (field[item_y + 2][item_x] != 0) {
                    if (item_x > HEIGTH - 1) {
                        item_type = 10;
                        item_y--;
                    }
                }
                else {
                    item_type = 10;
                }
            }
            else if (item_type == 10) {
                item_type = 11;
            }
            else if (item_type == 11) {
                item_type = 8;
            }
            else if (item_type == 12) {
                item_type = 13;
            }
            else if (item_type == 13) {
                item_type = 12;
            }
            else if (item_type == 14 && field[item_y + 1][item_x] == 0) {
                if (field[item_y + 2][item_x] != 0) {
                    if (item_x > HEIGTH - 1) {
                        item_type = 15;
                        item_y--;
                    }
                }
                else {
                    item_type = 15;
                }
            }
            else if (item_type == 15) {
                item_type = 14;
            }
            else if (item_type == 16 && field[item_y + 1][item_x] == 0) {
                if (field[item_y + 2][item_x] != 0) {
                    if (item_x > HEIGTH - 1) {
                        item_type = 17;
                        item_y--;
                    }
                }
                else {
                    item_type = 17;
                }
            }
            else if (item_type == 17) {
                item_type = 18;
            }
            else if (item_type == 18) {
                item_type = 19;
            }
            else if (item_type == 19) {
                item_type = 16;
            }
        }
        if (c == 's') {
            bool ich = false;
            ich = check_exit(item_x, item_y);
            if (!ich) {
                item_y++;
            }
        }
    }
    else {
        if (c == 'd' && item_x > 0) {
            int p = 0;
            if (item_type == 1 || item_type == 2 || item_type == 3 || item_type == 4 || item_type == 7 || item_type == 10 || item_type == 11 || item_type == 17) {
                for (int i = 0; i < item_size_y; i++) {
                    if (field[item_y - i][item_x - 1] != 0) {
                        p = 1;
                        break;
                    }
                }
            }
            else if (item_type == 5) {
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 6) {
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 9) {
                if (field[item_y - 1][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 8) {
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 12) {
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 13) {
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 14) {
                if (field[item_y - 1][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 15) {
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 16) {
                if (field[item_y - 1][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x - 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 18) {
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 19) {
                if (field[item_y][item_x] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x - 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x] != 0) {
                    p = 1;
                }
            }
            if (p == 0)
                item_x--;
        }
        if (c == 'a' && item_x < WIDTH - item_size_x) {
            int p = 0;
            if (item_type == 1 || item_type == 2 || item_type == 3 || item_type == 5 || item_type == 6 || item_type == 8 || item_type == 9 || item_type == 19) {
                for (int i = 0; i < item_size_y; i++) {
                    if (field[item_y - i][item_x + item_size_x] != 0) {
                        p = 1;
                        break;
                    }
                }
            }
            else if (item_type == 7) {
                if (field[item_y][item_x + 3] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 4) {
                if (field[item_y][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 2] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 11) {
                if (field[item_y][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 3] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 10) {
                if (field[item_y][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 12) {
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x + 3] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 13) {
                if (field[item_y][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 2] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 14) {
                if (field[item_y - 1][item_x + 3] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x + 2] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 15) {
                if (field[item_y][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 1] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 16) {
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x + 3] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 18) {
                if (field[item_y - 1][item_x + 3] != 0) {
                    p = 1;
                }
                if (field[item_y][item_x + 2] != 0) {
                    p = 1;
                }
            }
            else if (item_type == 17) {
                if (field[item_y][item_x + 1] != 0) {
                    p = 1;
                }
                if (field[item_y - 1][item_x + 2] != 0) {
                    p = 1;
                }
                if (field[item_y - 2][item_x + 1] != 0) {
                    p = 1;
                }
            }
            if (p == 0)
                item_x++;
        }
        if (c == 's' && item_type != 1) {
            if (item_type == 3) {
                if (field[item_y][item_x + 1] != 0 || field[item_y][item_x + 2] != 0 || field[item_y][item_x + 3] != 0 || item_x + 3 > WIDTH) {
                    if (field[item_y][item_x - 1] != 0 || item_x - 1 < 0) {
                        item_type = 3;
                    }
                    else {
                        item_x--;
                        if (field[item_y][item_x + 2] != 0 || field[item_y][item_x + 3] != 0 || item_x + 3 > WIDTH) {
                            if (field[item_y][item_x - 1] != 0 || item_x - 1 < 0) {
                                item_type = 3;
                                item_x++;
                            }
                            else {
                                item_x--;
                                if (field[item_y][item_x + 3] != 0 || item_x + 3 >= WIDTH) {
                                    if (field[item_y][item_x - 1] != 0 || item_x - 1 < 0) {
                                        item_type = 3;
                                        item_x += 2;
                                    }
                                    else {
                                        item_x--;
                                        item_type = 2;
                                    }
                                }
                                else {
                                    item_type = 2;
                                }
                            }
                        }
                    }
                }
                else {
                    item_type = 2;
                }
            }
            else if (item_type == 2 && field[item_y + 1][item_x] == 0 && field[item_y + 2][item_x] == 0 && field[item_y + 3][item_x] == 0) {
                item_type = 3;
            }
            else if (item_type == 4 && field[item_y + 1][item_x + 2] == 0 && field[item_y][item_x + 2] == 0) {
                item_type = 5;
            }
            else if (item_type == 5) {
                item_type = 6;
            }
            else if (item_type == 6) {
                item_type = 7;
            }
            else if (item_type == 7) {
                item_type = 4;
            }
            else if (item_type == 8 && field[item_y][item_x] == 0 && field[item_y + 1][item_x + 2] == 0 && field[item_y][item_x + 2] == 0) {
                item_type = 9;
            }
            else if (item_type == 9 && field[item_y + 1][item_x] == 0) {
                if (field[item_y + 2][item_x] != 0) {
                    if (item_x > HEIGTH - 1) {
                        item_type = 10;
                        item_y--;
                    }
                }
                else {
                    item_type = 10;
                }
            }
            else if (item_type == 10) {
                item_type = 11;
            }
            else if (item_type == 11) {
                item_type = 8;
            }
            else if (item_type == 12) {
                item_type = 13;
            }
            else if (item_type == 13) {
                item_type = 12;
            }
            else if (item_type == 14 && field[item_y + 1][item_x] == 0) {
                if (field[item_y + 2][item_x] != 0) {
                    if (item_x > HEIGTH - 1) {
                        item_type = 15;
                        item_y--;
                    }
                }
                else {
                    item_type = 15;
                }
            }
            else if (item_type == 15) {
                item_type = 14;
            }
            else if (item_type == 16 && field[item_y + 1][item_x] == 0) {
                if (field[item_y + 2][item_x] != 0) {
                    if (item_x > HEIGTH - 1) {
                        item_type = 17;
                        item_y--;
                    }
                }
                else {
                    item_type = 17;
                }
            }
            else if (item_type == 17) {
                item_type = 18;
            }
            else if (item_type == 18) {
                item_type = 19;
            }
            else if (item_type == 19) {
                item_type = 16;
            }
        }
        if (c == 'w') {
            bool ich = false;
            ich = check_exit(item_x, item_y);
            if (!ich) {
                item_y++;
            }
        }
    }
}
void process_kpop() {
    char c = 0;
    if (_kbhit()) c = _getch();
    if (c == 'y') {
        a = 1;
    }
    else if (c == 'n') {
        a = 0;
    }
}
void clear_field() {
    for (int i = 0; i < HEIGTH; i++) {
        for (int u = 0; u < WIDTH; u++) {
            field[i][u] = 0;
        }
    }
    return;
}
int main() {
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, 0x0002);
    cout << "XXXXXXX  XXXXXX  XXXXXXX  XXXXX   XXXXX  XXXXX" << "\r\n";
    SetConsoleTextAttribute(h, 0x0003);
    cout << "   X     XX         X     X    X    X    X    " << "\r\n";
    SetConsoleTextAttribute(h, 0x0004);
    cout << "   X     XX         X     X    X    X    X    " << "\r\n";
    SetConsoleTextAttribute(h, 0x0005);
    cout << "   X     XXXXXX     X     X    X    X    XXXXX" << "\r\n";
    SetConsoleTextAttribute(h, 0x0006);
    cout << "   X     XXXXXX     X     XXXXX     X        X" << "\r\n";
    SetConsoleTextAttribute(h, 0x0007);
    cout << "   X     XX         X     XXX       X        X" << "\r\n";
    SetConsoleTextAttribute(h, 0x0008);
    cout << "   X     XX         X     X  X      X        X" << "\r\n";
    SetConsoleTextAttribute(h, 0x0009);
    cout << "   X     XXXXXX     X     X   X   XXXXX  XXXXX" << "\r\n" << "\r\n";
    SetConsoleTextAttribute(h, 0x0002);
    cout << "                 PRESS T" << "\r\n";
    while (true) {
        char umlaut = 0;
        if (_kbhit()) umlaut = _getch();
        if (umlaut == 't') break;
    }

    system("CLS");   
    cout << "Choose difficulty:" << "\r\n";
    cout << "1.Can I play, Daddy?(only line and cube; very small speed) ";
    SetConsoleTextAttribute(h, 0x0004);
    if (l == 1) cout << "<<";
    SetConsoleTextAttribute(h, 0x0002);
    cout << "\r\n";
    cout << "2.Casual(small speed) ";
    SetConsoleTextAttribute(h, 0x0004);
    if (l == 2) cout << "<<";
    SetConsoleTextAttribute(h, 0x0002);
    cout << "\r\n";
    cout << "3.Mittel(medium speed) ";
    SetConsoleTextAttribute(h, 0x0004);
    if (l == 3) cout << "<<";
    SetConsoleTextAttribute(h, 0x0002);
    cout << "\r\n";
    cout << "4.Gamer(high speed) ";
    SetConsoleTextAttribute(h, 0x0004);
    if (l == 4) cout << "<<";
    SetConsoleTextAttribute(h, 0x0002);
    cout << "\r\n";
    cout << "5.Hardcore(reversed keys; high speed) ";
    SetConsoleTextAttribute(h, 0x0004);
    if (l == 5) cout << "<<";
    SetConsoleTextAttribute(h, 0x0002);
    cout << "\r\n" << "\r\n";
    cout << "Tap w and s to choose, tap e to confirm";
    while (true) {
        SetConsoleTextAttribute(h, 0x0002);
        char c = 0;
        if (_kbhit()) c = _getch();
        if (c == 'w') {
            l -= 1;
            if (l == 0) {
                l = 5;
            }
            system("CLS");
            cout << "Choose difficulty:" << "\r\n";
            cout << "1.Can I play, Daddy?(only line and cube; very small speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 1) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n";
            cout << "2.Casual(small speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 2) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n";
            cout << "3.Mittel(medium speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 3) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n";
            cout << "4.Gamer(high speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 4) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n";
            cout << "5.Hardcore(reversed keys; high speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 5) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n" << "\r\n";
            cout << "Tap w and s to choose, tap e to confirm";
        }
        else if (c == 's') {
            l += 1;
            if (l == 6) {
                l = 1;
            }
            system("CLS");
            cout << "Choose difficulty:" << "\r\n";
            cout << "1.Can I play, Daddy?(only line and cube; very small speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 1) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n";
            cout << "2.Casual(small speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 2) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n";
            cout << "3.Mittel(medium speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 3) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n";
            cout << "4.Gamer(high speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 4) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n";
            cout << "5.Hardcore(reversed keys; high speed) ";
            SetConsoleTextAttribute(h, 0x0004);
            if (l == 5) cout << "<<";
            SetConsoleTextAttribute(h, 0x0002);
            cout << "\r\n" << "\r\n";
            cout << "Tap w and s to choose, tap e to confirm";
        }
        else if (c == 'e') {
            break;
        }      
    }
    switch (l) {
    case 1:
        will = 850;
        break;
    case 2:
        will = 750;
        break;
    case 3:
        will = 650;
        break;
    case 4:
        will = 450;
        break;
    case 5:
        will = 250;
        break;
    }
    bool exit = false;
    do {
        if (item_type == 1) {
            item[0][0] = 1;
            item[0][1] = 1;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 1;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 0;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 2;
            item_size_y = 2;
        }
        if (item_type == 2) {
            item[0][0] = 1;
            item[0][1] = 1;
            item[0][2] = 1;
            item[0][3] = 1;
            item[1][0] = 0;
            item[1][1] = 0;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 0;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 4;
            item_size_y = 1;
        }
        else if (item_type == 3) {
            item[0][0] = 1;
            item[0][1] = 0;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 0;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 1;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 1;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 1;
            item_size_y = 4;
        }
        else if (item_type == 4) {
            item[0][0] = 1;
            item[0][1] = 0;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 0;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 1;
            item[2][1] = 1;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 2;
            item_size_y = 3;
        }
        else if (item_type == 8) {
        item[0][0] = 0;
        item[0][1] = 1;
        item[0][2] = 0;
        item[0][3] = 0;
        item[1][0] = 0;
        item[1][1] = 1;
        item[1][2] = 0;
        item[1][3] = 0;
        item[2][0] = 1;
        item[2][1] = 1;
        item[2][2] = 0;
        item[2][3] = 0;
        item[3][0] = 0;
        item[3][1] = 0;
        item[3][2] = 0;
        item[3][3] = 0;
        item_size_x = 2;
        item_size_y = 3;
        }
        else if (item_type == 12) {
        item[0][0] = 0;
        item[0][1] = 1;
        item[0][2] = 1;
        item[0][3] = 0;
        item[1][0] = 1;
        item[1][1] = 1;
        item[1][2] = 0;
        item[1][3] = 0;
        item[2][0] = 1;
        item[2][1] = 1;
        item[2][2] = 0;
        item[2][3] = 0;
        item[3][0] = 0;
        item[3][1] = 0;
        item[3][2] = 0;
        item[3][3] = 0;
        item_size_x = 3;
        item_size_y = 2;
        }
        else if (item_type == 14) {
        item[0][0] = 1;
        item[0][1] = 1;
        item[0][2] = 0;
        item[0][3] = 0;
        item[1][0] = 0;
        item[1][1] = 1;
        item[1][2] = 1;
        item[1][3] = 0;
        item[2][0] = 0;
        item[2][1] = 0;
        item[2][2] = 0;
        item[2][3] = 0;
        item[3][0] = 0;
        item[3][1] = 0;
        item[3][2] = 0;
        item[3][3] = 0;
        item_size_x = 3;
        item_size_y = 2;
        }
        else if (item_type == 16) {
        item[0][0] = 1;
        item[0][1] = 1;
        item[0][2] = 1;
        item[0][3] = 0;
        item[1][0] = 0;
        item[1][1] = 1;
        item[1][2] = 0;
        item[1][3] = 0;
        item[2][0] = 0;
        item[2][1] = 0;
        item[2][2] = 0;
        item[2][3] = 0;
        item[3][0] = 0;
        item[3][1] = 0;
        item[3][2] = 0;
        item[3][3] = 0;
        item_size_x = 3;
        item_size_y = 2;
        }
        generate_new_item();
        int p = 0;
        do {
            p += 1;
            system("CLS");  // Очистка всего экрана
            draw_field();
            remove_item_from_field(item_x, item_y, item);
            exit = check_exit(item_x, item_y);
            if (!exit) process_fall(&item_y);
            if (!exit) process_keyboard();
            if (item_type == 2) {
                item[0][0] = 1;
                item[0][1] = 1;
                item[0][2] = 1;
                item[0][3] = 1;
                item[1][0] = 0;
                item[1][1] = 0;
                item[1][2] = 0;
                item[1][3] = 0;
                item[2][0] = 0;
                item[2][1] = 0;
                item[2][2] = 0;
                item[2][3] = 0;
                item[3][0] = 0;
                item[3][1] = 0;
                item[3][2] = 0;
                item[3][3] = 0;
                item_size_x = 4;
                item_size_y = 1;
            }
            else if (item_type == 3) {
                item[0][0] = 1;
                item[0][1] = 0;
                item[0][2] = 0;
                item[0][3] = 0;
                item[1][0] = 1;
                item[1][1] = 0;
                item[1][2] = 0;
                item[1][3] = 0;
                item[2][0] = 1;
                item[2][1] = 0;
                item[2][2] = 0;
                item[2][3] = 0;
                item[3][0] = 1;
                item[3][1] = 0;
                item[3][2] = 0;
                item[3][3] = 0;
                item_size_x = 1;
                item_size_y = 4;
            }
            else if (item_type == 4) {
                item[0][0] = 1;
                item[0][1] = 0;
                item[0][2] = 0;
                item[0][3] = 0;
                item[1][0] = 1;
                item[1][1] = 0;
                item[1][2] = 0;
                item[1][3] = 0;
                item[2][0] = 1;
                item[2][1] = 1;
                item[2][2] = 0;
                item[2][3] = 0;
                item[3][0] = 0;
                item[3][1] = 0;
                item[3][2] = 0;
                item[3][3] = 0;
                item_size_x = 2;
                item_size_y = 3;
            }
            else if (item_type == 5) {
                item[0][0] = 0;
                item[0][1] = 0;
                item[0][2] = 1;
                item[0][3] = 0;
                item[1][0] = 1;
                item[1][1] = 1;
                item[1][2] = 1;
                item[1][3] = 0;
                item[2][0] = 0;
                item[2][1] = 0;
                item[2][2] = 0;
                item[2][3] = 0;
                item[3][0] = 0;
                item[3][1] = 0;
                item[3][2] = 0;
                item[3][3] = 0;
                item_size_x = 3;
                item_size_y = 2;
            }
            else if (item_type == 6) {
                item[0][0] = 1;
                item[0][1] = 1;
                item[0][2] = 0;
                item[0][3] = 0;
                item[1][0] = 0;
                item[1][1] = 1;
                item[1][2] = 0;
                item[1][3] = 0;
                item[2][0] = 0;
                item[2][1] = 1;
                item[2][2] = 0;
                item[2][3] = 0;
                item[3][0] = 0;
                item[3][1] = 0;
                item[3][2] = 0;
                item[3][3] = 0;
                item_size_x = 2;
                item_size_y = 3;
            }
            else if (item_type == 7) {
                item[0][0] = 1;
                item[0][1] = 1;
                item[0][2] = 1;
                item[0][3] = 0;
                item[1][0] = 1;
                item[1][1] = 0;
                item[1][2] = 0;
                item[1][3] = 0;
                item[2][0] = 0;
                item[2][1] = 0;
                item[2][2] = 0;
                item[2][3] = 0;
                item[3][0] = 0;
                item[3][1] = 0;
                item[3][2] = 0;
                item[3][3] = 0;
                item_size_x = 3;
                item_size_y = 2;
            }
            else if (item_type == 8) {
            item[0][0] = 0;
            item[0][1] = 1;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 0;
            item[1][1] = 1;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 1;
            item[2][1] = 1;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 2;
            item_size_y = 3;
            }
            else if (item_type == 9) {
            item[0][0] = 1;
            item[0][1] = 1;
            item[0][2] = 1;
            item[0][3] = 0;
            item[1][0] = 0;
            item[1][1] = 0;
            item[1][2] = 1;
            item[1][3] = 0;
            item[2][0] = 0;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 3;
            item_size_y = 2;
            }
            else if (item_type == 10) {
            item[0][0] = 1;
            item[0][1] = 1;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 0;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 1;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 2;
            item_size_y = 3;
            }
            else if (item_type == 11) {
            item[0][0] = 1;
            item[0][1] = 0;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 1;
            item[1][2] = 1;
            item[1][3] = 0;
            item[2][0] = 0;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 3;
            item_size_y = 2;
            }
            else if (item_type == 12) {
            item[0][0] = 0;
            item[0][1] = 1;
            item[0][2] = 1;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 1;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 1;
            item[2][1] = 1;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 3;
            item_size_y = 2;
            }
            else if (item_type == 13) {
            item[0][0] = 1;
            item[0][1] = 0;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 1;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 0;
            item[2][1] = 1;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 2;
            item_size_y = 3;
            }
            else if (item_type == 14) {
            item[0][0] = 1;
            item[0][1] = 1;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 0;
            item[1][1] = 1;
            item[1][2] = 1;
            item[1][3] = 0;
            item[2][0] = 0;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 3;
            item_size_y = 2;
            }
            else if (item_type == 15) {
            item[0][0] = 0;
            item[0][1] = 1;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 1;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 1;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 2;
            item_size_y = 3;
            }
            else if (item_type == 16) {
            item[0][0] = 1;
            item[0][1] = 1;
            item[0][2] = 1;
            item[0][3] = 0;
            item[1][0] = 0;
            item[1][1] = 1;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 0;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 3;
            item_size_y = 2;
            }
            else if (item_type == 19) {
            item[0][0] = 0;
            item[0][1] = 1;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 1;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 0;
            item[2][1] = 1;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 2;
            item_size_y = 3;
            }
            else if (item_type == 18) {
            item[0][0] = 0;
            item[0][1] = 1;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 1;
            item[1][2] = 1;
            item[1][3] = 0;
            item[2][0] = 0;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 3;
            item_size_y = 2;
            }
            else if (item_type == 17) {
            item[0][0] = 1;
            item[0][1] = 0;
            item[0][2] = 0;
            item[0][3] = 0;
            item[1][0] = 1;
            item[1][1] = 1;
            item[1][2] = 0;
            item[1][3] = 0;
            item[2][0] = 1;
            item[2][1] = 0;
            item[2][2] = 0;
            item[2][3] = 0;
            item[3][0] = 0;
            item[3][1] = 0;
            item[3][2] = 0;
            item[3][3] = 0;
            item_size_x = 2;
            item_size_y = 3;
            }
            if (!exit) check_field();
            if (!exit) Sleep(will); // Ждём некоторое время (треть секунды)
            insert_item_to_field(item_x, item_y, item, item_color);
            exit = check_exit(item_x, item_y);
        } while (!exit);
        if (p == 1) {
            Sleep(1000);
            system("CLS");
            Sleep(80);
            draw_field();
            Sleep(80);
            system("CLS");
            for (int i = 0; i < 8; i++) {
                if (points >= records[i]) {
                    for (int u = 6; u > i; u--) {
                        records[u] = records[u - 1];
                    }
                    records[i] = points;
                    break;
                }
            }
            SetConsoleTextAttribute(h, 0x0002);
            cout << " XXXXX     XXX    XXX   XXX  XXXXXX" << "\r\n";
            SetConsoleTextAttribute(h, 0x0003);
            cout << "XXXXXXX   XX XX   XXXX XXXX  XX    " << "\r\n";
            SetConsoleTextAttribute(h, 0x0004);
            cout << "XX   XX  XX   XX  XX XXX XX  XX    " << "\r\n";
            SetConsoleTextAttribute(h, 0x0005);
            cout << "XX       XX   XX  XX  X  XX  XXXXXX" << "\r\n";
            SetConsoleTextAttribute(h, 0x0006);
            cout << "XX       XXXXXXX  XX     XX  XXXXXX" << "\r\n";
            SetConsoleTextAttribute(h, 0x0007);
            cout << "XX  XXX  XX   XX  XX     XX  XX    " << "\r\n";
            SetConsoleTextAttribute(h, 0x0008);
            cout << "XX   XX  XX   XX  XX     XX  XX    " << "\r\n";
            SetConsoleTextAttribute(h, 0x0009);
            cout << "XXXXXXX  XX   XX  XX     XX  XXXXXX" << "\r\n" << "\r\n";
            SetConsoleTextAttribute(h, 0x0001);
            cout << " XXXXXXX  XX    XX  XXXXXX  XXXXX " << "\r\n";
            SetConsoleTextAttribute(h, 0x0002);
            cout << " XX   XX  XX    XX  XX      XX   X" << "\r\n";
            SetConsoleTextAttribute(h, 0x0003);
            cout << " XX   XX  XX    XX  XX      XX   X" << "\r\n";
            SetConsoleTextAttribute(h, 0x0004);
            cout << " XX   XX   XX  XX   XXXXXX  XXXXXX" << "\r\n";
            SetConsoleTextAttribute(h, 0x0005);
            cout << " XX   XX   XX  XX   XXXXXX  XXXX  " << "\r\n";
            SetConsoleTextAttribute(h, 0x0006);
            cout << " XX   XX    XXXX    XX      XX X  " << "\r\n";
            SetConsoleTextAttribute(h, 0x0007);
            cout << " XXXXXXX    XXXX    XX      XX  X " << "\r\n";
            SetConsoleTextAttribute(h, 0x0008);
            cout << " XXXXXXX     XX     XXXXXX  XX  XX" << "\r\n";
            cout << "Your records:" << "\r\n";
            for (int i = 0; i < 8; i++) {
                cout << i + 1 << ": " << records[i] << "\r\n";
            }
            cout << "\r\n" << "Do you want to play again? 'y/n'" << "\r\n";
            int y = 0;
            while (true) {            
                process_kpop();
                if (a != -1) {
                    break;
                }
            }
            if (a == 0) {
                
                break;
            }
            else {
                system("CLS");
                cout << "Choose difficulty:" << "\r\n";
                cout << "1.Can I play, Daddy?(only line and cube; very small speed) ";
                SetConsoleTextAttribute(h, 0x0004);
                if (l == 1) cout << "<<";
                SetConsoleTextAttribute(h, 0x0002);
                cout << "\r\n";
                cout << "2.Casual(small speed) ";
                SetConsoleTextAttribute(h, 0x0004);
                if (l == 2) cout << "<<";
                SetConsoleTextAttribute(h, 0x0002);
                cout << "\r\n";
                cout << "3.Mittel(medium speed) ";
                SetConsoleTextAttribute(h, 0x0004);
                if (l == 3) cout << "<<";
                SetConsoleTextAttribute(h, 0x0002);
                cout << "\r\n";
                cout << "4.Gamer(high speed) ";
                SetConsoleTextAttribute(h, 0x0004);
                if (l == 4) cout << "<<";
                SetConsoleTextAttribute(h, 0x0002);
                cout << "\r\n";
                cout << "5.Hardcore(reversed keys; high speed) ";
                SetConsoleTextAttribute(h, 0x0004);
                if (l == 5) cout << "<<";
                SetConsoleTextAttribute(h, 0x0002);
                cout << "\r\n" << "\r\n";
                cout << "Tap w and s to choose, tap e to confirm";
                while (true) {
                    SetConsoleTextAttribute(h, 0x0002);
                    char c = 0;
                    if (_kbhit()) c = _getch();
                    if (c == 'w') {
                        l -= 1;
                        if (l == 0) {
                            l = 5;
                        }
                        system("CLS");
                        cout << "Choose difficulty:" << "\r\n";
                        cout << "1.Can I play, Daddy?(only line and cube; very small speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 1) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n";
                        cout << "2.Casual(small speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 2) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n";
                        cout << "3.Mittel(medium speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 3) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n";
                        cout << "4.Gamer(high speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 4) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n";
                        cout << "5.Hardcore(reversed keys; high speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 5) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n" << "\r\n";
                        cout << "Tap w and s to choose, tap e to confirm";
                    }
                    else if (c == 's') {
                        l += 1;
                        if (l == 6) {
                            l = 1;
                        }
                        SetConsoleTextAttribute(h, 0x0002);
                        system("CLS");
                        cout << "Choose difficulty:" << "\r\n";
                        cout << "1.Can I play, Daddy?(only line and cube; very small speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 1) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n";
                        cout << "2.Casual(small speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 2) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n";
                        cout << "3.Mittel(medium speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 3) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n";
                        cout << "4.Gamer(high speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 4) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n";
                        cout << "5.Hardcore(reversed keys; high speed) ";
                        SetConsoleTextAttribute(h, 0x0004);
                        if (l == 5) cout << "<<";
                        SetConsoleTextAttribute(h, 0x0002);
                        cout << "\r\n" << "\r\n";
                        cout << "Tap w and s to choose, tap e to confirm";
                    }
                    else if (c == 'e') {
                        break;
                    }
                }
                switch (l) {
                case 1:
                    will = 850;
                    break;
                case 2:
                    will = 750;
                    break;
                case 3:
                    will = 650;
                    break;
                case 4:
                    will = 450;
                    break;
                case 5:
                    will = 250;
                    break;
                }
                a = -1;
                p = 0;
                points = 0;
                clear_field();
            }
        }
        item_color += 1;
        item_type += 1;
        if (item_type > 2 && l == 1) {
            item_type = 1;
        }
        if (item_type == 3) {
            item_type = 4;
        }
        if (item_type == 5 || item_type == 6 || item_type == 7) {
            item_type = 8;
        }
        if (item_type == 9 || item_type == 10 || item_type == 11) {
            item_type = 12;
        }
        if (item_type == 13) {
            item_type = 14;
        }
        if (item_type == 15) {
            item_type = 16;
        }
        if (item_type > 16) {
            item_type = 1;
        }
        if (item_color == 8) {
            item_color = 1;
        }
        exit = false;
        while (_kbhit()) {
            _getch();
        }
    } while (true);
    return 0;
}
