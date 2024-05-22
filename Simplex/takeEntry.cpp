#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

typedef struct FileContent
{
    string goal;
    vector<string> expressions;
};

FileContent readFile()
{
    FileContent fileContent;
    fileContent.goal = "";
    fileContent.expressions = vector<string>();
    ifstream file("expression.txt");
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << std::endl;
        ;
    }
    getline(file, fileContent.goal);    
    string line;
    while(getline(file, line)) {
        fileContent.expressions.push_back(line);
    }
    return fileContent;
}

void readGoal(string str) {
    string goal;
    int i = 0;
    while(str[i] != ' ') {
        goal += str[i];
        i++;
    }
    cout << goal << endl;
    if (goal == "min" || goal == "minimizar" || "Min") {
        cout << "Minimizar" << endl;
    }
    else if (goal == "max" ||goal == "maximizar" || "Max") {
        cout << "Maximizar" << endl;
    }
    else {
        cout << "Objetivo inválido" << endl;
    }
}

vector<string> splitStringIntoVector(const string& str) {

    stringstream ss(str);
    vector<string> result;
    string temp;
    while (getline(ss, temp)) {
        result.push_back(temp);
    }
    return result;
}

pair<float, int> parseTerm(const string& term) {
    size_t pos = term.find('x');
    if (pos == string::npos) {
        throw invalid_argument("Invalid term format");
    }
    float floatValue = stof(term.substr(0, pos));
    int indexValue = stoi(term.substr(pos + 1));
    return {floatValue, indexValue};
}

void readExpression(string str) {
    FileContent content;
    content = readFile();
    vector<string> items = splitStringIntoVector(content.goal);
    


    cout << "Objetivo: " << items[0] << endl;
}