#pragma once
#include <fstream>
#include <string>

std::string input_filename();
int input_line_length();
void open_file(std::ifstream& file, const std::string& filename);