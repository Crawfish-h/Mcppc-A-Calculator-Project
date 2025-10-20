#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <ranges>
#include <utility>
#include <concepts>
#include <functional>
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
        if (To_Digit(tokens[j]))
        {
            return true;
        }
    }

    return false;
}

bool Eval_All_Basic_Op(size_t i, std::vector<std::string>& tokens)
{
    bool do_Basic_Op = false;

    if (Eq_Multi<Or>(tokens[i], "+", "-", "*", "/", "%", "^"))
    {
        do_Basic_Op = true;
    }

    if (do_Basic_Op == true)
    {
        if (i == 0 || To_Digit(tokens[i - 1]) == false)
        {
            Print("Error: expected number value left of operator.\n");
            return false;
        }else if (i == tokens.size() - 1 || To_Digit(tokens[i + 1]) == false)
        {
            Print("Error: expected number value right of operator.\n");
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

template<Callable_Ts<double, double, double> F>
bool Eval_Basic_Op(std::vector<std::string>& tokens, const std::string& valid_Ops, F op_function)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        bool token_Is_Op = false;
        for (size_t j = 0; j < valid_Ops.size(); i++)
        {
            if (tokens[i] == valid_Ops[j])
                token_Is_Op = true;
        }

        if (token_Is_Op)
        {
            if (i == 0 || To_Digit(tokens[i - 1]) == false)
            {
                Print("Error: expected number value left of operator.\n");
                return false;
            }else if (i == tokens.size() - 1 || To_Digit(tokens[i + 1]) == false)
            {
                Print("Error: expected number value right of operator.\n");
                return false;
            }else
            {
                tokens[i + 1] = std::to_string(op_function(std::stod(tokens[i - 1]), std::stod(tokens[i + 1])));
                tokens.erase(tokens.begin() + i);
                tokens.erase(tokens.begin() + i - 1);
            }
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
        Print("Error: number of left parentheses, {}, does not match number of right parentheses, {}.\n", 
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

                Eval_All_Basic_Op(j, tokens);
            }

            tokens.erase(tokens.begin() + i);
            tokens.erase(tokens.begin() + right_Parens_Pos[0]);
            right_Parens_Pos.erase(right_Parens_Pos.begin());
            left_Parens_Pos.erase(left_Parens_Pos.begin() + i);
        }
    }

    has_Parens = false;
    return true;
}

bool Eval_Expons(std::vector<std::string>& tokens, bool& has_Expons)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "^")
        {
            if (i == 0 || To_Digit(tokens[i - 1]) == false)
            {
                Print("Error: expected number value left of operator.\n");
                return false;
            }else if (i == tokens.size() - 1 || To_Digit(tokens[i + 1]) == false)
            {
                Print("Error: expected number value right of operator.\n");
                return false;
            }else
            {
                tokens[i + 1] = std::to_string(std::pow(std::stod(tokens[i - 1]), std::stod(tokens[i + 1])));
                tokens.erase(tokens.begin() + i);
                tokens.erase(tokens.begin() + i - 1);
            }
        }
    }

    has_Expons = false;
    return true;
}

//template<Callable F>
//using Op_Func = [](double a, double b){ return F(a, b); };

void Eval_Tokens(std::vector<std::string>& tokens)
{
    // These booleans are used to implement PEMDAS.
    bool has_Parens = true;
    bool no_Expons = false;
    bool no_Mul_Div = false;
    bool no_Add_Sub = false;

    while (tokens.size() > 1)
    {
        for (size_t i = 0; i < tokens.size(); i++)
        {
            auto ret = [&]{ tokens.clear(); return Match_Break; };
            auto pow = [](double a, double b){ return std::pow(a, b); };
            auto add = [](double a, double b){ return a + b; };
            auto sub = [](double a, double b){ return a - b; };
            auto mul = [](double a, double b){ return a * b; };
            auto div = [](double a, double b){ return a / b; };
            auto mod = [](double a, double b){ return std::fmod(a, b); };

            Match(false)
            .Case(Eval_Parens(tokens, has_Parens), ret)
            .Case(Eval_Basic_Op(tokens, "^", pow), ret)
            .Case(Eval_Basic_Op(tokens, "*", mul), ret)
            .Case(Eval_Basic_Op(tokens, "/", div), ret)
            .Case(Eval_Basic_Op(tokens, "%", mod), ret)
            .Case(Eval_Basic_Op(tokens, "+", add), ret)
            .Case(Eval_Basic_Op(tokens, "-", sub), ret);
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
            /*if (i != input.size() - 1 && input[i] == 'p' && input[i + 1] == 'w')
            {
                tokens.push_back(std::string("") + input[i] + input[i + 1]);
            }*/

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
                case '^':
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