#include <iostream>
#include <locale>

#include "functions.h"
#include "classes.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");

	string filename = input_filename(); 
	int line_length = input_line_length(); 

	cout << "Результат выравнивания по центру с длиной строк " << line_length << " символов:\n\n";

	ifstream file; // Создаём переменнуб file типа ifstream (ifstream - для чтения из файла, ofstream - запись, fstream - и чтение, и запись)
	open_file(file, filename); // Открываем файл

	Words words_object; // Объект класса Words 
	words_object.get_words_from_file(file); // Создаём вектор слов

	Result result_object; // Объект класса Result
	result_object.make_lines(words_object, line_length); // Создаём строки по длине
	result_object.centere_and_print(line_length); // Выравниваем по центру и выводим
}