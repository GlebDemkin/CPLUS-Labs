#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <cctype>
#include <locale>

class Word {
public:
	Word() = default;
	explicit Word(std::string s) : text_(std::move(s)) {}

	const std::string& str() const { return text_; }
	std::size_t size() const { return text_.size(); }
	bool empty() const { return text_.empty(); }

private:
	std::string text_;
};

class Words {
public:
	Words() = default;

	void add(const Word& w) { data_.push_back(w); }
	void add(Word&& w) { data_.push_back(std::move(w)); }

	bool empty() const { return data_.empty(); }
	std::size_t size() const { return data_.size(); }

	auto begin() const { return data_.begin(); }
	auto end() const { return data_.end(); }

private:
	std::vector<Word> data_;
};

class Tokenizer {
public:
	static Words tokenize(const std::string& line) {
		Words words;
		std::istringstream iss(line);

		// Требование: разбить с помощью istream_iterator<string>
		for (auto it = std::istream_iterator<std::string>(iss);
			it != std::istream_iterator<std::string>(); ++it)
		{
			words.add(Word(*it));
		}
		return words;
	}
};

class WrappedLine {
public:
	WrappedLine() = default;

	bool empty() const { return words_.empty(); }
	std::size_t wordsCount() const { return words_.size(); }

	void addWord(const Word& w) { words_.push_back(w); }
	void addWord(Word&& w) { words_.push_back(std::move(w)); }

	std::size_t contentLength() const {
		if (words_.empty()) return 0;
		std::size_t len = 0;
		for (const auto& w : words_) len += w.size();
		len += (words_.size() - 1); // пробелы между словами
		return len;
	}

	std::string join() const {
		std::string out;
		out.reserve(contentLength());

		bool first = true;
		for (const auto& w : words_) {
			if (!first) out.push_back(' ');
			out += w.str();
			first = false;
		}
		return out;
	}

private:
	std::vector<Word> words_;
};

class WordWrapper {
public:
	explicit WordWrapper(std::size_t width) : width_(width) {
		if (width_ == 0) throw std::invalid_argument("Width must be > 0");
	}

	std::vector<WrappedLine> wrap(const Words& words) const {
		std::vector<WrappedLine> result;
		WrappedLine current;

		for (const auto& w : words) {
			if (w.size() > width_) {
				flushIfNotEmpty(result, current);

				// Режем длинное "слово" на куски ширины
				for (const auto& chunk : splitLongWord(w)) {
					WrappedLine single;
					single.addWord(chunk);
					result.push_back(std::move(single));
				}
				continue;
			}

			if (current.empty()) {
				current.addWord(w);
				continue;
			}

			const std::size_t candidateLen = current.contentLength() + 1 + w.size();
			if (candidateLen <= width_) {
				current.addWord(w);
			}
			else {
				result.push_back(std::move(current));
				current = WrappedLine{};
				current.addWord(w);
			}
		}

		flushIfNotEmpty(result, current);
		return result;
	}

private:
	std::size_t width_;

	static void flushIfNotEmpty(std::vector<WrappedLine>& out, WrappedLine& line) {
		if (!line.empty()) {
			out.push_back(std::move(line));
			line = WrappedLine{};
		}
	}

	std::vector<Word> splitLongWord(const Word& w) const {
		std::vector<Word> chunks;
		const std::string& s = w.str();

		for (std::size_t pos = 0; pos < s.size(); pos += width_) {
			chunks.emplace_back(s.substr(pos, width_));
		}
		return chunks;
	}
};

class CenterAligner {
public:
	explicit CenterAligner(std::size_t width) : width_(width) {}

	std::string center(const std::string& content) const {
		if (content.size() >= width_) return content; // на всякий случай

		std::size_t totalPad = width_ - content.size();
		std::size_t leftPad = totalPad / 2;
		std::size_t rightPad = totalPad - leftPad;

		return std::string(leftPad, ' ') + content + std::string(rightPad, ' ');
	}

private:
	std::size_t width_;
};

class TextFormatter {
public:
	explicit TextFormatter(std::size_t width)
		: width_(width), wrapper_(width), aligner_(width) {
	}

	void formatAndPrintLine(const std::string& line, std::ostream& out) const {
		if (isBlank(line)) {
			out << "\n";
			return;
		}

		Words words = Tokenizer::tokenize(line);

		// Если в строке нет пробелов, Tokenizer вернёт 1 "слово",
		// если оно длиннее width — wrapper_ разрежет на куски.
		auto wrapped = wrapper_.wrap(words);

		for (const auto& wl : wrapped) {
			const std::string content = wl.join();
			out << aligner_.center(content) << "\n";
		}
	}

private:
	std::size_t width_;
	WordWrapper wrapper_;
	CenterAligner aligner_;

	static bool isBlank(const std::string& s) {
		for (unsigned char c : s) {
			if (!std::isspace(c)) return false;
		}
		return true;
	}
};

static std::size_t parseWidth(const char* arg) {
	std::string s(arg);
	if (s.empty()) throw std::invalid_argument("Width is empty");

	std::size_t pos = 0;
	unsigned long long val = 0;
	try {
		val = std::stoull(s, &pos, 10);
	}
	catch (...) {
		throw std::invalid_argument("Width must be a positive integer");
	}
	if (pos != s.size()) throw std::invalid_argument("Width must be a positive integer");
	if (val == 0) throw std::invalid_argument("Width must be > 0");
	if (val > static_cast<unsigned long long>(std::numeric_limits<std::size_t>::max()))
		throw std::invalid_argument("Width is too large");
	return static_cast<std::size_t>(val);
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	try {
		std::string filename;
		std::size_t width = 0;

		if (argc == 3) {
			filename = argv[1];
			width = parseWidth(argv[2]);
		}
		else {
			// Запуск из IDE без аргументов — спросим параметры
			std::cout << "Enter input file name: ";
			std::getline(std::cin, filename);

			std::cout << "Enter line width: ";
			std::string w;
			std::getline(std::cin, w);
			width = parseWidth(w.c_str());
		}

		std::ifstream in(filename);
		if (!in) {
			std::cerr << "Error: cannot open file: " << filename << "\n";
			return 2;
		}

		TextFormatter formatter(width);

		std::string line;
		while (std::getline(in, line)) {
			formatter.formatAndPrintLine(line, std::cout);
		}

		return 0;
	}
	catch (const std::exception& ex) {
		std::cerr << "Fatal error: " << ex.what() << "\n";
		return 3;
	}
}