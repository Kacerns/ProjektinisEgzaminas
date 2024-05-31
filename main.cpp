#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <map>
#include <set>
#include <regex>

std::string filterPunctuation(const std::string &word) {
    std::string filtered_word;
    for (char ch : word) {
        if (!std::ispunct(ch) || ch == '.' || ch == '/' || ch == ':') {
            filtered_word += ch;
        }
    }
    return filtered_word;
}

bool isURL(const std::string &word) {
    const std::regex url_pattern(R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt)))");
    return std::regex_search(word, url_pattern);
}

int main() {
    std::ifstream fd("text.txt");
    std::ofstream fr("output.txt");
    std::ofstream fa("url.txt");
    if (!fd) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    std::map<std::string, std::pair<int, std::set<int>>> word_map;
    std::map<std::string, std::pair<int, std::set<int>>> url_map;
    std::string line;
    int line_number = 0;

    while (std::getline(fd, line)) {
        ++line_number;
        std::istringstream iss(line);
        std::string word;

        while (iss >> word) {
            std::string filtered_word = filterPunctuation(word);
            if (!filtered_word.empty()) {
                if (isURL(filtered_word)) {
                    url_map[filtered_word].first++;
                    url_map[filtered_word].second.insert(line_number);
                } else {
                    word_map[filtered_word].first++;
                    word_map[filtered_word].second.insert(line_number);
                }
            }
        }
    }

    fd.close();

    // Print the words and their counts
    fr << "Words:\n";
    for (const auto &entry : word_map) {
      if(entry.second.first>1){
        fr << entry.first << ": " << entry.second.first << " [lines: ";
        for (const int &ln : entry.second.second) {
            fr << ln << " ";
        }
        fr << "]" << std::endl;
      }
    }

    // Print the URLs and their counts
    fa << "\nURLs:\n";
    for (const auto &entry : url_map) {
        fa << entry.first << std::endl;
    }
    fa.close();
    fr.close();
    return 0;
}
