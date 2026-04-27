#include "functions.h"
#include <iostream>

using namespace std;

// Функция, которая запрашивает название файла
string input_filename() {
	string filename;
	cout << "Введите имя файла: ";
	cin >> filename;
	return filename;
}

// Функция, которая запрашивает длину строки
int input_line_length() {
	int line_length;
	cout << "Введите желаемую длину строки: ";
	cin >> line_length;

	if (line_length <= 0) {
		cout << "Некорректный размер строки!\n";
		exit(1);
	}
	return line_length;
}

// Функция, которая открывает файл
void open_file(ifstream& file, const string& filename) {
	file.open(filename);

	if (!file.is_open()) {
		cout << "Ошибка открытия файла!\n";
		exit(1);
	}
}