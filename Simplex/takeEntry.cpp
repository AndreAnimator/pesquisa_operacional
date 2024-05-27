#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

typedef struct
{
    string goal;
    vector<string> expressions;
} FileContent;

vector<string> split(const string &s, char delimiter)
{
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void parseString(const string &input, double &value, int &index, bool debug = false)
{
    // Default values
    value = 1.0;
    index = 0;

    // Verifica se a string é vazia
    if (input.empty())
    {
        return;
    }

    size_t pos = 0;
    bool isNegative = false;

    // Verifica se o termo é negativo
    if (input[pos] == '+' || input[pos] == '-')
    {
        isNegative = (input[pos] == '-');
        pos++;
    }

    // Acha a posição de 'x'
    size_t xPos = input.find('x', pos);
    if (xPos == string::npos)
    {
        cerr << "Invalid format: 'x' not found" << endl;
        return;
    }

    // Extrai o valor
    if (xPos > pos)
    {
        string valueStr = input.substr(pos, xPos - pos);
        value = stof(valueStr);
    }

    // Aplica o sinal negativo
    if (isNegative)
    {
        value = -value;
    }

    // Extrai o indice
    string indexStr = input.substr(xPos + 1);
    if (!indexStr.empty())
    {
        index = stoi(indexStr);
    }

    if (debug)
    {
        // Output for debugging
        cout << "Input: " << input << ", Value: " << value << ", Index: " << index << endl;
    }
}

vector<string> tokenizeExpression(const string &expression)
{
    vector<string> tokens;
    istringstream iss(expression);
    string token;

    while (getline(iss, token, ' '))
    {
        // Remove espaços em branco
        token.erase(remove(token.begin(), token.end(), ' '), token.end());

        if (token[0] == 'x')
        {
            tokens.push_back("+1.0" + token);
        }
        else if (token[0] == '+' || token[0] == '-')
        {
            tokens.push_back(token);
        }
        else
        {
            tokens.push_back("+" + token);
        }
    }
    vector<string> newTokens;
    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].size() > 2)
        {
            newTokens.push_back(tokens[i]);
        }
    }
    return newTokens;
}

vector<double> catchExpression(string str, int tam = 0)
{
    int i = 0;
    size_t equalPos = str.find('=');
    string substr = str.substr(equalPos + 1, str.size());
    cout << substr << endl;
    vector<string> tokens = tokenizeExpression(substr);
    int index;
    double value;
    vector<pair<double, int>> values; // valores dos coeficientes
    for (int i = 0; i < tokens.size(); i++)
    {
        parseString(tokens[i], value, index);
        values.push_back(make_pair(value, index));
    }
    if (tam == 0)
    {
        tam = values.size();
    }
    vector<double> result(tam, 0.0);
    for (int i = 0; i < values.size(); i++)
    {
        result[values[i].second - 1] = values[i].first;
    }
    return result;
}

FileContent readFile()
{
    FileContent fileContent;
    fileContent.goal = "";
    fileContent.expressions = vector<string>();
    ifstream file("entrada.txt");
    if (!file.is_open())
    {
        cerr << "Erro ao abrir o arquivo." << endl;
    }
    getline(file, fileContent.goal);
    string line;
    while (getline(file, line))
    {
        fileContent.expressions.push_back(line);
    }
    return fileContent;
}

int main()
{
    FileContent fileContent = readFile();
    vector<double> goal = catchExpression(fileContent.goal);
    vector<vector<double>> expressions;
    // Tem que tratar para ignorar o sinal de maior e menor igual no final
    // for (int i = 0; i < fileContent.expressions.size(); i++)
    // {
    //     expressions.push_back(catchExpression(fileContent.expressions[i], goal.size()));
    // }
    
    for (int i = 0; i < goal.size(); i++)
    {
        cout << goal[i] << " ";
    }
    return 0;
}