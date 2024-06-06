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
//com|org|net|edu|gov|io|co|lt|site|xyz|co\.uk|fr|

bool URL(const string &CheckIfURL){
    // regex URLType(R"((http://|https://|www\.|[a-zA-Z0-9.-]+\.(com|org|net|edu|gov|io|co|lt)))");
    regex URLType(R"((http://|https://|www\.)[a-zA-Z0-9.-]+\.[a-zA-Z0-9]{2,}|[a-zA-Z.-]+\.([a-zA-Z]{2,})|[a-zA-Z._%+-]+\.([a-z]{2,4}))");
    return regex_search(CheckIfURL, URLType);
}

int main(int argc, const char *argv[]){

    unordered_map<string, pair< int, set<int> >> words;
    unordered_map<string, pair< int, set<int> >> urlcount;
    unordered_map<string, pair< int, set<int> >> kultur;


    string current_line;
    int current_line_number = 0;

    ifstream text("text.txt");
    string search = "kultūr";
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
            string temp = word;
            std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c){ return std::tolower(c); });
            if(temp.find(search) != string::npos){
                kultur.emplace(word, std::make_pair(0, set<int>{}));
                kultur.at(word).first++;
                kultur.at(word).second.insert(current_line_number);
            }
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

    outputFile <<endl;

    outputFile << "Kultura: "<< endl;
    for (auto instance : kultur){
        outputFile << instance.first << ": " << instance.second.first << " [Mentioned in: ";
        for (int lines : instance.second.second){
            outputFile << lines << " ";
        }
        outputFile << "]" << endl;
    }

    outputFile.close();

    return 0;

}
