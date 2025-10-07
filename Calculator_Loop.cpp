#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <type_traits>
#include "Print.h"
#include "Match.h"
#include "Util.h"

bool Is_Digit(const std::string& string)
{
    for (char ch : string)
    {
        if (isdigit(ch) == false && ch != '.')
        {
            return false;
        }
    }

    return true;
}

bool Find_Value(const std::vector<std::string>& tokens, size_t i, bool forwards = true)
{
    int iter_Val = 1;
    if (forwards == false) iter_Val = -1;
    for (ssize_t j = i; j < tokens.size() && j > -1; j += iter_Val)
    {
        if (j == i) continue;

        // WIP: add check for functions that return types that cannot be compared to an int.
        if (Is_Digit(tokens[j]))
        {
            return true;
        }
    }

    return false;
}

void Eval_Tokens(std::vector<std::string>& tokens)
{
    while (tokens.size() > 1)
    {
        for (size_t i = 0; i < tokens.size(); i++)
        {
            bool do_Basic_Op = false;

            if (Eq_Multi<Or>(tokens[i], "+", "-", "*", "/"))
            {
                do_Basic_Op = true;
            }

            if (do_Basic_Op == true)
            {
                if (i == 0 || Is_Digit(tokens[i - 1]) == false)
                {
                    tokens.clear();
                    Print("Error: expected number left of operator.\n");
                    return;
                }else if (i == tokens.size() - 1 || Is_Digit(tokens[i + 1]) == false)
                {
                    tokens.clear();
                    Print("Error: expected number right of operator.\n");
                    return;
                }else
                {
                    double value = 0.0;
                    double left_Val = std::stod(tokens[i - 1]);
                    double right_Val = std::stod(tokens[i + 1]);

                    Match(tokens[i])
                    .Case("+", [&]{ value = left_Val + right_Val; })
                    .Case("-", [&]{ value = left_Val - right_Val; })
                    .Case("*", [&]{ value = left_Val * right_Val; })
                    .Case("/", [&]{ value = left_Val / right_Val; })
                    .Default([]
                    { 
                        Print("Error: invalid input.\n");
                        return;
                    });

                    tokens[i + 1] = std::to_string(value);
                    tokens.erase(tokens.begin() + i);
                    tokens.erase(tokens.begin() + i - 1);
                }
            }
        }
    }

    Print("{}\n", tokens[0]);
    tokens.clear();
}

void Calculator_Loop()
{
    std::string input = "";
    std::vector<std::string> tokens;
    bool expect_Op = false;
    bool expect_Num = false;
    double left_Num = 0;
    double right_Num = 0;

    while (input != "exit")
    {
        input = "";
        std::getline(std::cin, input);
        std::string input_Scanned = "";

        for (char chai : input)
        {
            switch(chai)
            {
                case '+':
                case '-':
                case '*':
                case '/':
                case '%':
                case '^':
                case '(':
                case ')':
                case '<':
                case '>':
                case ',':
                    if (input_Scanned != "")
                        tokens.push_back(input_Scanned);
                    input_Scanned = "";

                    tokens.push_back(std::string("") + chai);
                    break;
                default:
                    input_Scanned += chai;
                    break;
            }
        }

        if (input_Scanned[input_Scanned.size() - 1] != ' ' && input_Scanned != "")
        {
            tokens.push_back(input_Scanned);
        }

        for (auto& tok : tokens)
        {
            for (size_t i = 0; i < tok.size(); i++)
                if (tok[i] == ' ')
                    tok.erase(tok.begin() + i);
        }
        
        Eval_Tokens(tokens);
    }
}