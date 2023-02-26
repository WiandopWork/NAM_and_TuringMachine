// ИНСТРУКЦИЯ ЗАПУСКА ЭМУЛЯТОРА НОРМАЛЬНОГО АЛГОРИТМА МАРКОВА
// Функция NMAstep() выводит пошаговую работу программы
// Функция NMAfull() выводит результат работу программы
//
// Иструкция по вводу данных:
// 1. Ввод системы подстановок. Ввод каждой подстановки вводится в скобках в формате (строка операция строка). Пример (a -> b)
// Примечание к пункту 1. Ввод условий подстановки через пробел. Ввод подстановок через Enter. -> - подстановка; => - подстановка с остановом.
// Окончание ввода обозначается '(!)'
// 2. Ввод размеров таблицы. Первое число - высота, второе - ширина. В размеры таблицы входят все состояния и колонка пустоты
// 3. Ввод таблицы программы. Каждая ячейка вводится в формате symbol,move,status. Между запятыми нет пробелов. Между ячейками пробел
// Ячейка в которую нет доступа программе обозначается как #,N,текущий статус
// 4. Ввод input

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct Node {
    string old_word;
    string new_word;
    int operation; // 0 - подстановка; 1 - подстановка с остановом
};

vector <int> prefix_function (string);
int findKMP(string, string);
void input(vector <Node> &);
void print(vector <Node> &);
void NMAstep(vector <Node> &, string &);
void NMAfull(vector <Node> &, string &);

int main() {
    vector <Node> system;
    string in_out;

    input(system);
    cout << "Enter input:";
    cin >> in_out;
    cout << endl;

    print(system);
    cout << endl;
    NMAstep(system, in_out);
    cout << endl;
    cout << "Res: " << in_out << endl;
    return 0;
}

vector <int> prefix_function (string str) {
    int size = (int) str.length();
    vector<int> pi(size);

    for (int i = 1; i < size; i++) {
        int j = pi[i - 1];

        while (j > 0 && str[i] != str[j]) {
            j = pi[j - 1];
        }

        if (str[i] == str[j]) {
            j++;
        }

        pi[i] = j;
    }

    return pi;
}

int findKMP(string in_out, string find) {
    string buf = find + "@" + in_out;

    vector <int> pref = prefix_function(buf);

    for (int i = find.size() + 1; i < buf.size(); i++) {
        if (pref[i] == find.size()) {
            return i - 2 * find.size();
        }
    }

    return -1;
}

void input(vector <Node> &system) {
    cout << "Enter the substitution system" << endl;
    while (true) {
        string buf;
        Node temp;

        cin >> buf;
        if (buf[1] == '!') {
            cout << endl;
            return;
        }

        // Условия безусловной вставки
        if (buf[1] == '-') {
            temp.old_word = '#';
            temp.operation = 0;

            cin >> buf;
            buf.pop_back();

            temp.new_word = buf;

            system.push_back(temp);
            continue;
        } else if (buf[1] == '=') {
            temp.old_word = '#';
            temp.operation = 1;

            cin >> buf;
            buf.pop_back();

            temp.new_word = buf;

            system.push_back(temp);
            continue;
        }

        buf.erase(0, 1);
        temp.old_word = buf;

        cin >> buf;
        if (buf[0] == '-') {
            temp.operation = 0;
        } else if (buf[0] == '=') {
            temp.operation = 1;
        }

        if (buf[buf.size() - 1] == ')') {
            temp.new_word = '#';

            system.push_back(temp);
            continue;
        }

        cin >> buf;
        buf.pop_back();
        temp.new_word = buf;

        system.push_back(temp);
    }
}

void print(vector <Node> &system) {
    for (int i = 0; i < system.size(); i++) {
        if (system[i].old_word == "#") {
            cout << "  ";
        } else {
            cout << system[i].old_word << " ";
        }

        if (system[i].operation == 0) {
            cout << "-> ";
        } else {
            cout << "=> ";
        }

        if (system[i].new_word == "#") {
            cout << " " << endl;
        } else {
            cout << system[i].new_word << endl;
        }
    }
}

void NMAstep(vector <Node> &system, string &in_out) {
    int position = 0;
    int iter = 0;
    while (true) {
        if (position > system.size() - 1) {
            return;
        }

        if (system[position].old_word == "#") {
            for (int i = 1; i <= system[position].new_word.size(); i++) {
                in_out.insert(in_out.begin(), system[position].new_word[system[position].new_word.size() - i]);
            }

            cout << iter << " " << in_out << endl;
            iter++;

            if (system[position].operation == 0) {
                position = 0;
                continue;
            } else {
                return;
            }
        }

        if (findKMP(in_out, system[position].old_word) == -1) {
            position++;
            continue;
        }

        size_t pos = findKMP(in_out, system[position].old_word);

        if (system[position].new_word != "#") {
            in_out.replace(pos, system[position].old_word.length(), system[position].new_word);
        } else {
            in_out.erase(pos, system[position].old_word.length());
        }

        cout << iter << " " << in_out << endl;
        iter++;

        if (system[position].operation == 0) {
            position = 0;
            continue;
        } else {
            return;
        }
    }
}

void NMAfull(vector <Node> &system, string &in_out) {
    int position = 0;
    while (true) {
        if (position > system.size() - 1) {
            return;
        }

        if (system[position].old_word == "#") {
            for (int i = 1; i <= system[position].new_word.size(); i++) {
                in_out.insert(in_out.begin(), system[position].new_word[system[position].new_word.size() - i]);
            }

            if (system[position].operation == 0) {
                position = 0;
                continue;
            } else {
                return;
            }
        }

        if (findKMP(in_out, system[position].old_word) == -1) {
            position++;
            continue;
        }

        size_t pos = findKMP(in_out, system[position].old_word);

        if (system[position].new_word != "#") {
            in_out.replace(pos, system[position].old_word.length(), system[position].new_word);
        } else {
            in_out.erase(pos, system[position].old_word.length());
        }

        if (system[position].operation == 0) {
            position = 0;
            continue;
        } else {
            return;
        }
    }
}