#include "classes.h"
#include <iostream>
#include <sstream>
#include <iterator>

using namespace std;

void Words::get_words_from_file(ifstream& file) {
	string line;
	while (getline(file, line)) {
		istringstream stream_line(line);
		istream_iterator<string> begin(stream_line);
		istream_iterator<string> end;
		words.insert(words.end(), begin, end);
	}
}

void Result::refresh_buffer(string& buffer_line) {
	if (!buffer_line.empty()) {
		result.push_back(buffer_line);
		buffer_line.clear();
	}
}

void Result::make_lines(const Words& words_object, int line_length) {
	string buffer_line;
	int word_length;
	int index = 0;

	for (const auto& word : words_object.words) {
		word_length = word.length();

		if (word_length > (size_t)line_length) {
			refresh_buffer(buffer_line);

			while (word_length > (size_t)line_length) {
				result.push_back(word.substr(index, line_length));
				index += line_length;
				word_length -= line_length;
			}

			buffer_line = word.substr(index);
			index = 0;
		}
		else if (buffer_line.empty()) {
			buffer_line = word;
		}
		else if ((int)(buffer_line.length() + word.length() + 1) <= line_length) {
			buffer_line += ' ' + word;
		}
		else {
			refresh_buffer(buffer_line);
			buffer_line = word;
		}
	}

	refresh_buffer(buffer_line);
}

void Result::centere_and_print(int line_length) {
	for (auto& line : result) {
		size_t space_num = line_length - line.length();
		if ((space_num % 2) != 0) {
			line = string(((space_num / 2) + 1), ' ') + line + string((space_num / 2), ' ');
		}
		else {
			line = string((space_num / 2), ' ') + line + string((space_num / 2), ' ');
		}
		cout << line << endl;
	}
}