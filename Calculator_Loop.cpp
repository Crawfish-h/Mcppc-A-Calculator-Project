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

std::tuple<std::string&, size_t> Find_Value(std::vector<std::string>& tokens, size_t i, bool left = true)
{    
    int iter_Val = 1;
    std::string found_Token = "aaa";
    if (left == false) iter_Val = -1;
    for (ssize_t j = i; j < tokens.size() && j > -1; j += iter_Val)
    {
        if (j == i) continue;

        // WIP: add check for functions that return types that cannot be compared to an int.
        if (To_Digit(tokens[j]))
        {
            return { tokens[j], j };
        }else if (tokens[j] != "")
        {
            found_Token = tokens[j];
            break;
        }
    }

    if (left == true)
        Print("Error: could not find left digit. Instead, found: {}", found_Token);
    else
        Print("Error: could not find right digit. Instead, found: {}", found_Token);

    return { tokens[i], i };
}

template<Callable_Ts<double, double, double>... Block_Ts>
bool Eval_Basic_Op(std::vector<std::string>& tokens, const std::string& valid_Ops, Block_Ts... op_Blocks)
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        bool token_Is_Op = false;
        
        size_t j = 0;
        for (; j < valid_Ops.size(); j++)
        {
            std::string current_Op = " ";
            current_Op[0] = valid_Ops[j];
            if (tokens[i] == current_Op)
            {
                token_Is_Op = true;
                break;
            }
        }

        if (token_Is_Op)
        {
            if (i == 0 || To_Digit(tokens[i - 1]) == false)
            {
                Print("Error: expected number value left of operator.\n");
                Print("Tokens: ");
                Print("{}\n", tokens);
                return false;
            }else if (i == tokens.size() - 1 || To_Digit(tokens[i + 1]) == false)
            {
                Print("Error: expected number value right of operator.\n");
                Print("Tokens: ");
                Print("{}\n", tokens);
                return false;
            }else
            {
                std::vector<std::function<double(double, double)>> blocks = { op_Blocks... };
                tokens[i + 1] = std::to_string(blocks[j](std::stod(tokens[i - 1]), std::stod(tokens[i + 1])));
                
                tokens.erase(tokens.begin() + i);
                tokens.erase(tokens.begin() + i - 1);
                i -= 2;
            }
        }
    }

    return true;
}

std::optional<double> Eval_Tokens(std::vector<std::string>& tokens, bool clear_Tokens = true, bool print_Value = true);

bool Eval_Parens(std::vector<std::string>& tokens)
{
    std::vector<std::string> stack;

    for (const auto& token : tokens)
    {
        if (token == ")")
        {
            // Collect everything inside the parentheses
            std::vector<std::string> inner;
            while (!stack.empty() && stack.back() != "(")
            {
                inner.insert(inner.begin(), stack.back());
                stack.pop_back();
            }

            if (stack.empty())
            {
                Print("Mismatched parentheses: missing '('.\n");
                return false;
            }

            // Remove the '('
            stack.pop_back();

            // Evaluate the inner expression
            double value = 0.0f;
            if (auto option = Eval_Tokens(inner))
            {
                value = option.value();
            }else
            {
                Print("Could not evaluate parentheses.\n");
                return false;
            }
                
            // Push result back as a string
            stack.push_back(std::to_string(value));
        }else
        {
            stack.push_back(token);
        }
    }

    // If any parentheses remain, it's an error
    for (const auto& t : stack)
    {
        if (t == "(" || t == ")")
        {
            Print("Mismatched parentheses in expression.\n");
            return false;
        }
    }

    // Copy evaluated expression to tokens vector.
   if (stack.size() > 0) tokens = stack;
   return true;
}

std::optional<double> Eval_Tokens(std::vector<std::string>& tokens, bool clear_Tokens, bool print_Value)
{
    while (tokens.size() > 1)
    {
        for (size_t i = 0; i < tokens.size(); i++)
        {
            bool error = false;
            auto ret = [&]{ tokens.clear(); error = true; return Match_Break; };
            auto pow = [](double a, double b){ return std::pow(a, b); };
            auto add = [](double a, double b){ return a + b; };
            auto sub = [](double a, double b){ return a - b; };
            auto mul = [](double a, double b){ return a * b; };
            auto div = [](double a, double b){ return a / b; };
            auto mod = [](double a, double b){ return std::fmod(a, b); };

            Match(false)
            .Case(Eval_Parens(tokens), ret)
            .Case(Eval_Basic_Op(tokens, "^", pow), ret)
            .Case(Eval_Basic_Op(tokens, "*/%", mul, div, mod), ret)
            .Case(Eval_Basic_Op(tokens, "+-", add, sub), ret);

            if (error == true) return {};
        }
    }

    Remove_Trailing_Zeros(tokens[0]);
    if (print_Value == true) Print("{}\n", tokens[0]);
    if (clear_Tokens == true) tokens.clear();
    return std::stod(tokens[0]);
}

// Returns false if tokens contains no negative numbers.
bool Eval_Minus(std::vector<std::string>& tokens)
{
    std::vector<std::string> evaled_Tokens;
    bool add_Minus = false; // If true, next token is negative.
    bool found_Negative_Token = false;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (Is_Subtraction(tokens, i))
        {
            evaled_Tokens.push_back(tokens[i]);
        }else if (tokens[i] == "-")
        {
            add_Minus = true;
            continue;
        }else if (add_Minus == false)
        {
            evaled_Tokens.push_back(tokens[i]);
        }

        if (add_Minus == true)
        {
            found_Negative_Token = true;
            tokens[i].insert(0, "-");
            add_Minus = false;
            evaled_Tokens.push_back(tokens[i]);
        }
    }

    if (found_Negative_Token)
        tokens = evaled_Tokens;

    return found_Negative_Token;
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
            switch(input[i])
            {
                case '-':
                case '+':
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

        Print("Before: ");
        Print("{}\n", tokens);

        Eval_Minus(tokens);

        Print("After: ");
        Print("{}\n", tokens);

        Eval_Tokens(tokens);
    }
}