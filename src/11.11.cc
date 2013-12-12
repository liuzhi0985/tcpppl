// Created by Samvel Khalatyan on Dec 12, 2013
// Copyright (c) 2013 Samvel Khalatyan. All rights reserved
//
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <iostream>

#include "interface/11.11.h"

using std::endl;
using std::istream;
using std::string;

using Token = ex_11_11::Token;
using Kind = Token::Kind;
using Itokenstream = ex_11_11::Itokenstream;
using Calculator = ex_11_11::Calculator;

// -- Token stream
//
Itokenstream::Itokenstream(std::istream &is):
    _is{&is}
{}

istream &Itokenstream::stream() const noexcept
{
    return *_is;
}

void Itokenstream::stream(istream &is) noexcept
{
    _is = &is;
}

Token &Itokenstream::get()
    // read next token
{
    char ch {0};
    do
    {
        if (!_is->get(ch))
            break;
    } while(ch != '\n' && isspace(ch));

    switch(ch)
    {
        case 0:
            _current.kind = {Kind::end};
            break;

        case '\n':
        case ';':
            _current = {Kind::print};
            break;

        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '(':
        case ')':
            _current = {static_cast<Kind>(ch)};
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case '.':
            _is->putback(ch);
            *_is >> _current.value;
            _current.kind = Kind::number;
            break;

        default:
            if (isalpha(ch))
            {
                string name {ch};
                while(_is->get(ch) && isalpha(ch))
                    name += ch;

                _is->putback(ch);

                _current = {Kind::name, name};
            }
            else
            {
                // failed input
                _current.kind = Kind::end;
            }

            break;
    }

    return _current;
}

const Token &Itokenstream::current() const noexcept
{
    return _current;
}


// -- Calculator
//
Calculator::Calculator():
    _its{new Itokenstream{std::cin}}
{
}

void Calculator::run()
{
    while(not _errors)
    {
        _its->get();

        if (Kind::end == _its->current().kind)
            break;

        if (Kind::print == _its->current().kind)
            continue;

        std::cout << expression(false) << endl;
    }
}

Calculator::value_type Calculator::expression(const bool &get)
{
    value_type left = term(get);
    while(true)
    {
        switch(_its->current().kind)
        {
            case Kind::plus:
                left += term(true);
                break;

            case Kind::minus:
                left -= term(true);
                break;

            default:
                return left;
        }
    }
}

Calculator::value_type Calculator::term(const bool &get)
{
    value_type left = primary(get);
    while(true)
    {
        switch(_its->current().kind)
        {
            case Kind::multiply:
                left *= primary(true);
                break;

            case Kind::divide:
                if (value_type value = primary(true))
                    left /= value;
                else
                    error("division by zero");

                break;

            default:
                return left;
        }
    }
}

Calculator::value_type Calculator::primary(const bool &get_next)
{
    if (get_next)
        _its->get();

    switch(_its->current().kind)
    {
        case Kind::number:
            {
                value_type value {_its->current().value};
                _its->get();

                return value;
            }

        case Kind::minus:
            return -primary(true);

        case Kind::name:
            {
                value_type &value = _table[_its->current().name];
                if (Kind::assign == _its->get().kind)
                    value = expression(true);

                return value;
            }

        case Kind::lp:
            {
                value_type value = expression(true);
                if (Kind::rp == _its->current().kind)
                {
                    _its->get();    // eat ')'

                    return value;
                }

                // fall through
            }

        default:
            error("unsupported primary");
    }

    return 1;
}

void Calculator::error(const std::string &message)
{
    ++_errors;

    std::cerr << message << endl;
}
