// ИНСТРУКЦИЯ ЗАПУСКА ЭМУЛЯТОРА МАШИНЫ ТЬЮРИНГА
// Функция TuringMachineStep() выводит пошаговую работу программы
// Функция TuringMachineFull() выводит результат работу программы
//
// Иструкция по вводу данных:
// 1. Ввод алфавита. Символы вводятся через проблем. Конец ввода обозначается символом '!'
// 2. Ввод размеров таблицы. Первое число - высота, второе - ширина. В размеры таблицы входят все состояния и колонка пустоты
// 3. Ввод таблицы программы. Каждая ячейка вводится в формате symbol,move,status. Между запятыми нет пробелов. Между ячейками пробел
// Ячейка в которую нет доступа программе обозначается как #,N,текущий статус
// 4. Ввод input
// 5. Ввод начального статуса программы.

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct Node {
    char symbol;
    int status;
    char move;
};

struct Table {
    vector <vector <Node> > program;
    int height;
    int width;
};

void read_alphabet(set <char> &);
void read_table(Table &, set <char>);
void print_table(Table, set <char>);
void input(vector <char> &);
void TuringMachineStep(vector <char> &, Table, set <char>);
void TuringMachineFull(vector <char> &, Table, set <char>);

int main() {
    set <char> alphabet;
    Table table;
    vector <char> tape;

    read_alphabet(alphabet);
    read_table(table, alphabet);
    cout << endl;
    print_table(table, alphabet);
    cout << endl;
    input(tape);
    TuringMachineStep(tape, table, alphabet);

    for (auto x: tape) {
        cout << x;
    }

    cout << endl;
    return 0;
}


void read_alphabet(set <char> &alphabet) {
    cout << "Enter the alphabet. Complete the input '!'." << endl;
    char buf = '0';
    while (buf != '!') {
        cin >> buf;

        if (buf == '!') {
            break;
        }

        alphabet.insert(buf);
    }

    alphabet.insert('#');
    return;
}

void read_table(Table &table, set <char> alphabet) {
    cout << "Enter the sizes of the table. The first number is height, the second is width." << endl;
    cin >> table.height >> table.width;

    cout << "Enter the program" << endl;
    vector <Node> temp;
    for (int i = 0; i < table.height; i++) {
        for (int j = 0; j < table.width; j++) {
            string buf;
            cin >> buf;

            Node new_node;

            new_node.symbol = buf[0];
            buf.erase(0, 1);
            buf.erase(0, 1);

            int flag = 0;
            for (auto x : alphabet) {
                if (x == new_node.symbol) {
                    flag++;
                }
            }

            if (flag == 0) {
                cerr << "The symbol is missing in the alphabet" << endl;
                j--;
                continue;
            }

            new_node.move = buf[0];
            buf.erase(0, 1);
            buf.erase(0, 1);

            new_node.status = 0;

            for (int i = 0; i < buf.size(); i++) {
                new_node.status = new_node.status + (buf[i] - '0') * pow(10, i);
            }

            if (new_node.status > table.height) {
                cerr << "The status is missing" << endl;
                j--;
                continue;
            }

            temp.push_back(new_node);
        }

        table.program.push_back(temp);
        temp.erase(temp.begin(), temp.end());
    }

    return;
}

void print_table(Table table, set <char> alphabet) {
    cout << "      ";
    for (auto x: alphabet) {
        if (x == '#') {
            continue;
        }

        cout << x << "       ";
    }

    cout << '#';

    cout << endl;

    for (int i = 0; i < table.height; i++) {
        cout << i << "   ";
        for (int j = 0; j < table.width; j++) {
            cout << table.program[i][j].symbol << "," << table.program[i][j].move << "," << table.program[i][j].status << "   ";
        }

        cout << endl;
    }
}

void input(vector <char> &tape) {
    cout << "Enter input: ";
    string buf; cin >> buf;

    for (int i = 0; i < buf.size(); i++) {
        tape.push_back(buf[i]);
    }

    return;
}

void TuringMachineFull(vector <char> &tape, Table table, set <char> alphabet) {
    cout << "Enter the start status: ";
    int cur_status; cin >> cur_status;
    int position = 0;

    while (true) {
        char cur_symbol = tape[position];

        int cur_pos = 0;
        for (auto x : alphabet) {
            if (x == cur_symbol) {
                break;
            }

            cur_pos++;
        }

        if (cur_symbol == '#') {
            cur_pos = table.width - 1;
        } else {
            cur_pos--;
        }

        //cout << "INFO: " << position << " " << cur_symbol << " " << cur_pos << " " << cur_status << endl;
        tape[position] = table.program[cur_status][cur_pos].symbol;

        if (tape[position] == '#') {
            if (position == 0) {
                tape.erase(tape.begin());
                position--;
            }
        }

        if (table.program[cur_status][cur_pos].move == '!') {
            cur_status = table.program[cur_status][cur_pos].status;
            return;
        }

        if (table.program[cur_status][cur_pos].move == 'N') {
            continue;
        }

        if (table.program[cur_status][cur_pos].move == 'R') {
            if (position == tape.size() - 1) {
                tape.push_back('#');
            }

            position++;
        }

        if (table.program[cur_status][cur_pos].move == 'L') {
            if (position == 0) {
                tape.insert(tape.begin(), '#');
                position++;
            }

            if (cur_symbol == '#') {
                tape.erase(tape.end() - 1);
            }

            position--;
        }

        cur_status = table.program[cur_status][cur_pos].status;
    }
}

void TuringMachineStep(vector <char> &tape, Table table, set <char> alphabet) {
    cout << "Enter the start status: ";
    int cur_status; cin >> cur_status;
    int position = 0;

    while (true) {
        char cur_symbol = tape[position];

        int cur_pos = 0;
        for (auto x : alphabet) {
            if (x == cur_symbol) {
                break;
            }

            cur_pos++;
        }

        if (cur_symbol == '#') {
            cur_pos = table.width - 1;
        } else {
            cur_pos--;
        }

        //cout << "INFO: " << position << " " << cur_symbol << " " << cur_pos << " " << cur_status << endl;
        tape[position] = table.program[cur_status][cur_pos].symbol;

        if (tape[position] == '#') {
            if (position == 0) {
                tape.erase(tape.begin());
                position--;
            }
        }

        if (table.program[cur_status][cur_pos].move == '!') {
            cur_status = table.program[cur_status][cur_pos].status;

            cout << table.program[cur_status][cur_pos].symbol << "," << table.program[cur_status][cur_pos].move << "," << table.program[cur_status][cur_pos].status << "   ";

            for (auto x: tape) {
                cout << x;
            }

            cout << endl;

            return;
        }

        if (table.program[cur_status][cur_pos].move == 'N') {
            cur_status = table.program[cur_status][cur_pos].status;

            cout << table.program[cur_status][cur_pos].symbol << "," << table.program[cur_status][cur_pos].move << "," << table.program[cur_status][cur_pos].status << "   ";

            for (auto x: tape) {
                cout << x;
            }

            cout << endl;

            continue;
        }

        if (table.program[cur_status][cur_pos].move == 'R') {
            if (position == tape.size() - 1) {
                tape.push_back('#');
            }

            position++;
        }

        if (table.program[cur_status][cur_pos].move == 'L') {
            if (position == 0) {
                tape.insert(tape.begin(), '#');
                position++;
            }

            if (tape[position] == '#') {
                if (position == tape.size() - 1) {
                    tape.erase(tape.end() - 1);
                }
            }

            position--;
        }

        cout << table.program[cur_status][cur_pos].symbol << "," << table.program[cur_status][cur_pos].move << "," << table.program[cur_status][cur_pos].status << "   ";

        for (auto x: tape) {
            cout << x;
        }

        cout << endl;

        cur_status = table.program[cur_status][cur_pos].status;
    }
}