#pragma once

enum Comp { And, Or };

template<Comp comp = And, typename T, typename... Args>
bool Eq_Multi(T&& value, Args&&... args)
{
    if (comp == And)
        return ((value == args) && ...);
    else
        return ((value == args) || ...);
}