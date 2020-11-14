#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

string* result;

vector<string> readStrings(const char* path) {
    ifstream in(path);
    vector<string> result(0);
    string line;
    while (in >> line)
        result.push_back(line);
    in.close();
    return result;
}

string getCyph(char c, vector<string> keys) {
    return keys[c - 'a'];
}

void encryptWord(string word, vector<string> keys, int i) {
    string en = "";
    for (int i = 0; i < word.size(); i++)
        en += getCyph(word[i], keys);
    result[i] = en;
}

void threadEncrypt(string text, int numOfThreads, vector<string> keys) {
    auto startTime = high_resolution_clock::now();
    
    // main loop
    result = new string[numOfThreads];
    int wordLength = ceil(text.size() * 1.0 / numOfThreads);

    string* words = new string[numOfThreads];
    int begin = 0;
    int i = 0;
    while (1) {
        // get the task
        string line = text.substr(begin, wordLength);
        thread t(encryptWord, line, keys, i);
        t.join();
        begin += wordLength;
        ++i;
        if (begin >= text.size()) { // no more tasks
            break;
        }
    }

    delete[] words;

    cout << "Encrypted message: ";
    for (int i = 0; i < numOfThreads; i++)
        cout << result[i];
    cout << endl;

    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);
    cout << "Time spent on parallel calculation (number of threads - " << numOfThreads << ") : " << duration.count() * 1.0 / 1000000 << endl << endl;
}


int main(int argc, char* argv[]) {
    // argv[1] - input file
    // argv[2] - keys file
    if (argc < 3){
	cout << "Not enough console arguments" << endl;
	cin.get();
	return 0;
    }
    if (argc > 3){
	cout << "Too many console arguments" << endl;
	cin.get();
	return 0;
    }
    auto keys = readStrings(argv[2]);
    auto strings = readStrings(argv[1]);
    string text = "";
    for (string s : strings)
        text += s;
    cout << "Initial message: " << text << endl;
    for (int i = 0; i < sqrt(text.size()); i++)
        threadEncrypt(text, i + 1, keys);
    cin.get();
    delete[] result;
    return 0;
}
