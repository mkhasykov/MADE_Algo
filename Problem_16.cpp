/*
Найдите все вхождения шаблона в строку. Длина шаблона – p, длина строки ­– n. Время O(n + p), доп. память – O(p).
Вариант 1. С помощью префикс-функции (алгоритм Кнута-Морриса-Пратта).
*/

#include<fstream>
#include<iostream>
#include<vector>
#include<string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

vector<int> KMP(const string& pattern, const string& text) {

	vector<int> pi_pattern(pattern.length(), 0); // pi-function values for pattern
	unsigned int pi_prev; // previous value of pi-function
	vector<int> matches;

	for (unsigned int i = 1; i < pattern.length(); ++i) {
		pi_prev = pi_pattern[i - 1];
		while (pi_prev > 0 && pattern[i] != pattern[pi_prev])
			pi_prev = pi_pattern[pi_prev - 1];

		if (pattern[i] == pattern[pi_prev])
			pi_prev++;

		pi_pattern[i] = pi_prev;
	}

	pi_prev = 0;
	for (unsigned int i = 0; i < text.length(); ++i) {
		while (pi_prev > 0 && pattern[pi_prev] != text[i])
			pi_prev = pi_pattern[pi_prev-1];

		if (pattern[pi_prev] == text[i])
			pi_prev++;

		if (pi_prev == pattern.length()) {
			matches.push_back(i - pattern.length() + 1);
		}
	}
	return matches;
}

int main() {
	std::ifstream infile("input.txt");
	std::ofstream outfile("output.txt");

	string pattern, text;
	infile >> pattern >> text;

	vector<int> matches = KMP(pattern, text);
	for (unsigned int i=0; i < matches.size(); ++i) {
		outfile << matches[i] << " ";
	}

	infile.close();
	outfile.close();

	return 0;
}
