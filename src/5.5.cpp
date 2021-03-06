// Created by Samvel Khalatyan on Nov 24, 2013
// Copyright (c) 2013 Samvel Khalatyan. All rights reserved
//
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
//
// (*1.5)   Write a program that reads a name (a string) and an age (an int)
//          from the standard input stream cin. Then output a message
//          including the name and age to the standard output stream cout.

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{
    cout << "input name: ";

    string name;
    cin >> name;
    name[0] = toupper(name[0]); // capitalize a first letter in the name

    cout << "input age: ";
    int age;
    cin >> age;

    cout << "Your name is " << name <<
        " and you are " << age << " year(s) old." << endl;
}
