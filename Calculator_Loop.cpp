#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <type_traits>
#include <utility>
#include <optional>
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

bool Find_Next_Value(const std::vector<std::string>& tokens, size_t i)
{
    for (auto& token : tokens)
    {
        
    }
}

void Eval_Tokens(std::vector<std::string>& tokens)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "+")
        {
            if (i == 0 || String_Is_Digit(tokens[i]) == false)
            {
                tokens.clear();
                Print("Error: expected number left of operator.\n");
                return;
            }else if (i == tokens.size() - 1 || String_Is_Digit(tokens[i + 1]) == false)
            {
                tokens.clear();
                Print("Error: expected number right of operator.\n");
                return;
            }else
            {
                double sum = std::stod(tokens[i - 1]) + std::stod(tokens[i + 1]);
                tokens[i + 1] = std::to_string(sum);
            }
            
        }
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


    std::pair<int, bool> value(1910, true);
    int a = 10;
    int b = 2002;

    Match(a, value)
    .Case(10, {191, false}, [&]{ Print("int a: {}\n", a); })
    .Case(100, {191, true}, [&]{ Print("int b: {}\n", b); })
    .Case(10, {191, true}, []{ Print("2 + 6: {}\n", 2 + 6); })
    .Default([]{ Print("Default\n"); });

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