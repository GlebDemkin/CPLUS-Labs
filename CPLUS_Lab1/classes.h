#pragma once
#include <fstream>
#include <string>
#include <vector>

class Words {
public:
	std::vector<std::string> words;
	void get_words_from_file(std::ifstream& file);
};

class Result {
private:
	std::vector<std::string> result;
	void refresh_buffer(std::string& buffer_line);
public:
	void make_lines(const Words& words_object, int line_length);
	void centere_and_print(int line_length);
};