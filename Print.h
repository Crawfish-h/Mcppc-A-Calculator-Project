#pragma once
#include <string>
#include <format>
#include <iostream>
#include <type_traits>

template<typename T>
concept Iterable = requires(T t)
{
    requires (!std::is_same<T, std::string>::value);
    t.begin();
    t.end();
} && !std::same_as<std::remove_cvref_t<T>, std::string>;

template<typename... Args>
void Print_Container(const std::string& message, Args&&... args)
{
    std::cout << std::vformat(message, std::make_format_args(args...));
}

// 1 + cos(2, 3, 4)
template<Iterable... Args>
void Print_Container(const std::string& message, Args&&... args)
{
    std::cout << "Iterable\n";
    std::string next_Line = "";
    std::string msg_No_Nl = message; // message with no new line.
    if (message.contains("\n"))
    {
        next_Line = "\n";
        msg_No_Nl.erase(message.find("\n"), next_Line.size());
    }

    std::cout << "[";
    ([&]
    {
        for (auto it = args.begin(); it != args.end(); it++)
        {
            std::cout << std::vformat(msg_No_Nl, std::make_format_args(*it));
            if (it != args.end() - 1)
                std::cout << ", ";
        }
    }(), ...);
    std::cout << "]";

    std::cout << next_Line;
}

template<typename... Args>
void Print(const std::string& message, Args&&... args)
{
    size_t args_Amount = 0;
    ([&]
    {
        args;
        args_Amount++;
    }(), ...);

    if (args_Amount > 0)
    {
        Print_Container(message, args...);
    }else
    {
        std::cout << message;
    }
}