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

bool isLogicalOperator(string str)
{
    return str == ">=" || str == "<=" || str == "=";
}

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

    // Check if the string is empty
    if (input.empty())
    {
        cerr << "Input string is empty." << endl;
        return;
    }

    size_t pos = 0;
    bool isNegative = false;

    // Check if the term is negative
    if (input[pos] == '+' || input[pos] == '-')
    {
        isNegative = (input[pos] == '-');
        pos++;
    }

    // Find the position of 'x'
    size_t xPos = input.find('x', pos);
    if (xPos == string::npos)
    {
        cerr << "Invalid format: 'x' not found in '" << input << "'" << endl;
        return;
    }

    // Extract the value
    if (xPos > pos)
    {
        string valueStr = input.substr(pos, xPos - pos);
        try
        {
            value = stof(valueStr);
        }
        catch (const invalid_argument &e)
        {
            cerr << "Invalid_argument error: Unable to convert '" << valueStr << "' to float in input: " << input << endl;
            return;
        }
        catch (const out_of_range &e)
        {
            cerr << "Out_of_range error: '" << valueStr << "' is out of range in input: " << input << endl;
            return;
        }
    }

    // Apply the negative sign if needed
    if (isNegative)
    {
        value = -value;
    }

    // Extract the index
    string indexStr = input.substr(xPos + 1);
    if (!indexStr.empty())
    {
        try
        {
            index = stoi(indexStr);
        }
        catch (const invalid_argument &e)
        {
            cerr << "Invalid_argument error: Unable to convert '" << indexStr << "' to int in input: " << input << endl;
            return;
        }
        catch (const out_of_range &e)
        {
            cerr << "Out_of_range error: '" << indexStr << "' is out of range in input: " << input << endl;
            return;
        }
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
        // Remove spaces and ensure token is not empty
        token.erase(remove(token.begin(), token.end(), ' '), token.end());
        if (!token.empty())
        {
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


vector<double> catchExpression(string str, int& max_index, bool isExpression = true, bool debug = false, int tam = 0)
{
    int i = 0;
    size_t limitPosition;
    string substr = "";
    if (!isExpression)
    {
        limitPosition = str.find('=');
        substr = str.substr(limitPosition + 1);
    }
    limitPosition = str.find(isLogicalOperator(str));
    substr = str.substr(0, limitPosition);
    if (debug) cout << "Substr: " << substr << endl;

    vector<string> tokens = tokenizeExpression(substr);
    int index;
    double value;
    vector<pair<double, int>> values; // valores dos coeficientes
    for (i = 0; i < tokens.size(); i++)
    {
        parseString(tokens[i], value, index, debug);
        if (index > max_index) max_index = index;
        values.push_back(make_pair(value, index));
    }
    if (tam == 0) tam = values.size();
    vector<double> result(tam, 0.0);
    for (i = 0; i < values.size(); i++)
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
    int max_index = 0;
    vector<double> goal = catchExpression(fileContent.goal, max_index, false, true, 0);
    vector<vector<double>> expressions(max_index, vector<double>(0));
    for (int i = 0; i < fileContent.expressions.size(); i++)
    {
        expressions.push_back(catchExpression(fileContent.expressions[i], max_index, true, true, max_index));
    }
    for (int i = 0; i < goal.size(); i++)
    {
        cout << goal[i] << " ";
    }
    printf("\n");
    for (int i = 0; i < expressions.size(); i++)
    {
        for (int j = 0; j < expressions[i].size(); j++)
        {
            cout << expressions[i][j] << " ";
        }
        printf("\n");
    }
    return 0;
}