#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <type_traits>
#include <ranges>
#include "Print.h"
#include "Match.h"
#include "Util.h"

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

bool Eval_Basic_Op(size_t i, std::vector<std::string>& tokens)
{
    bool do_Basic_Op = false;

    if (Eq_Multi<Or>(tokens[i], "+", "-", "*", "/", "%", "pw"))
    {
        do_Basic_Op = true;
    }

    if (do_Basic_Op == true)
    {
        if (i == 0 || Is_Digit(tokens[i - 1]) == false)
        {
            Print("tokens: {}\n", Is_Digit(tokens[i - 1]) == false);
            tokens.clear();
            Print("Error: expected number left of operator.\n");
            return false;
        }else if (i == tokens.size() - 1 || Is_Digit(tokens[i + 1]) == false)
        {
            tokens.clear();
            Print("Error: expected number right of operator.\n");
            return false;
        }else
        {
            double value = 0.0;
            double left_Val = 0.0;
            double right_Val = 0.0;

            try
            {
                left_Val = std::stod(tokens[i - 1]);
                right_Val = std::stod(tokens[i + 1]);
            }catch(const std::invalid_argument& ia)
            {
                Print("Exeception thrown: 'std::invalid_argument'\n");
                Print("left_Val as string: {}\n", tokens[i - 1]);
                Print("right_Val as string: {}\n", tokens[i + 1]);
                Print("tokens: ");
                Print("{}\n", tokens);
                return false;
            }
            

            Match(tokens[i])
            .Case("+", [&]{ value = left_Val + right_Val; })
            .Case("-", [&]{ value = left_Val - right_Val; })
            .Case("*", [&]{ value = left_Val * right_Val; })
            .Case("/", [&]{ value = left_Val / right_Val; })
            .Case("%", [&]{ value = std::fmod(left_Val, right_Val); })
            .Case("pw", [&]{ value = std::pow(left_Val, right_Val); })
            .Default([]
            { 
                Print("Error: expected basic operator: +, -, *, /, %, or pw.\n");
                return false;
            });

            tokens[i + 1] = std::to_string(value);
            tokens.erase(tokens.begin() + i);
            tokens.erase(tokens.begin() + i - 1);
        }
    }

    return true;
}

bool Eval_Parens(std::vector<std::string>& tokens, bool& has_Parens)
{
    std::vector<size_t> right_Parens_Pos;
    std::vector<size_t> left_Parens_Pos;
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "(")
            left_Parens_Pos.push_back(i);
        else if (tokens[i] == ")")
            right_Parens_Pos.push_back(i);
    }

    if (left_Parens_Pos.size() != right_Parens_Pos.size())
    {
        Print("Error: number of left parentheses, {}, does not match number of right parentheses, {}.", 
            left_Parens_Pos.size(), right_Parens_Pos.size());

        return false;
    }

    while (right_Parens_Pos.size() > 0)
    {
        for (size_t i : left_Parens_Pos | std::views::reverse)
        {
            for (size_t j = i + 1; j < right_Parens_Pos[0] && j < tokens.size(); j++)
            {
                if (tokens[j] == ")")
                    break;

                Eval_Basic_Op(j, tokens);
            }

            tokens.erase(tokens.begin() + i);
            tokens.erase(tokens.begin() + right_Parens_Pos[0]);
            right_Parens_Pos.erase(right_Parens_Pos.begin());
            left_Parens_Pos.erase(left_Parens_Pos.begin() + i);
        }

        Print("right_Parens_Pos: ");
        Print("{}\n", right_Parens_Pos);
    }

    has_Parens = false;
    return true;
}

void Eval_Tokens(std::vector<std::string>& tokens)
{
    // These booleans are used to implement PEMDAS.
    bool has_Parens = true;
    bool has_Exponents = true;
    bool has_Mul_Div = true;
    bool has_Add_Sub = true;
    while (tokens.size() > 1)
    {
        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (has_Parens)
            {
                if (not Eval_Parens(tokens, has_Parens)) return;
            }else
            {
                if (not Eval_Basic_Op(i, tokens)) return;
            }
        }
    }

    Remove_Trailing_Zeros(tokens[0]);
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

        for (size_t i = 0; i < input.size(); i++)
        {
            if (i != input.size() - 1 && input[i] == 'p' && input[i + 1] == 'w')
            {
                tokens.push_back(std::string("") + input[i] + input[i + 1]);
            }

            switch(input[i])
            {
                case '+':
                case '-':
                case '*':
                case '/':
                case '%':
                case '(':
                case ')':
                case '<':
                case '>':
                case ',':
                    if (input_Scanned != "")
                        tokens.push_back(input_Scanned);
                    input_Scanned = "";

                    tokens.push_back(std::string("") + input[i]);
                    break;
                default:
                    if (input[i] != ' ')
                        input_Scanned += input[i];
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