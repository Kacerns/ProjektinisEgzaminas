#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <regex>
#include <unordered_map>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::regex;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ispunct;
using std::regex_search;
using std::unordered_map;
using std::pair;
using std::set;

string FilterWord(const string &wordToFilter){
    string result;
    for(char letter : wordToFilter){
        if(!ispunct(letter)){
            result.push_back(letter);
        }
    }
    return result;
}

bool URL(const string &CheckIfURL){
    regex URLType(R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt)))");
    return regex_search(CheckIfURL, URLType);
}

int main(int argc, const char *argv[]){

    unordered_map<string, pair< int, set<int> >> words;
    unordered_map<string, pair< int, set<int> >> urlcount;

    string current_line;
    int current_line_number = 0;

    ifstream text("text.txt");

    while(std::getline(text, current_line)){
        current_line_number++;
        istringstream iss(current_line);
        string parse;

        while(iss >> parse){
            if(parse.empty()){
                continue;
            }
            if(URL(parse)){
                urlcount.emplace(parse, std::make_pair(0, set<int>{}));
                urlcount.at(parse).first++;
                urlcount.at(parse).second.insert(current_line_number);
                continue;
            }
            string word = FilterWord(parse);
            words.emplace(word, std::make_pair(0, set<int>{}));
            words.at(word).first++;
            words.at(word).second.insert(current_line_number);
        }
    }
    text.close();

    ofstream outputFile("WordCount.txt");

    outputFile << "Words: "<< endl;
    for (auto instance : words){
        if(instance.second.first>1){
            outputFile << instance.first << ": " << instance.second.first << " [Mentioned in: ";
            for (int lines : instance.second.second){
                outputFile << lines << " ";
            }
            outputFile << "]" << endl;
        }
    }

    outputFile <<endl;

    outputFile << "URL: "<< endl;
    for (auto instance : urlcount){
        outputFile << instance.first << ": " << instance.second.first << " [Mentioned in: ";
        for (int lines : instance.second.second){
            outputFile << lines << " ";
        }
        outputFile << "]" << endl;
    }

    return 0;

}
