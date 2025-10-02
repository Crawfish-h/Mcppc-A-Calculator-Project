#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <type_traits>
#include <utility>
#include "Print.h"
#include "Match.h"

bool String_Is_Digit(const std::string& string)
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

void Eval_Tokens(std::vector<std::string>& tokens, bool expect_Right_Num = false)
{
    std::pair<int, bool> Value(100, true);
    Match(Value, Case());

    for (const std::string& token : tokens)
    {
        
    }
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
        std::getline(std::cin, input);
        std::string input_Scanned = "";

        for (char chai : input)
        {
            switch(chai)
            {
                case ' ':
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
            Print("input_Scanned: {}\n", input_Scanned);
            tokens.push_back(input_Scanned);
        }

        std::cout << input_Scanned << "\n";
        Print("Tokens: "); 
        Print("{}\n", tokens);
        
        Eval_Tokens(tokens);
    }
}