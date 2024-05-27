#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// Função para transformar a expressão em tokens
vector<string> tokenizeExpression(const string& expression) {
    vector<string> tokens;
    istringstream iss(expression);
    string token;

    while (getline(iss, token, ' ')) {
        // Remove espaços em branco
        token.erase(remove(token.begin(), token.end(), ' '), token.end());

        if (token.size() > 2)
        {
            if (token[0] == '+' || token[0] == '-')
            {
                tokens.push_back(token);
            }
            else
            {
                // Adiciona o sinal '+' se não houver sinal antes do termo
                tokens.push_back("+" + token);
            }
        }
    }

    return tokens;
}

int main() {
    string expression = "x1 + 3x2 - 1.2x3 + 5x4";
    vector<string> tokens = tokenizeExpression(expression);

    // Imprime os tokens
    for (const string& token : tokens) {
        cout << token << endl;
    }

    return 0;
}
