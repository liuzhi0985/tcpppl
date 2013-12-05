// Created by Samvel Khalatyan on Dec 5, 2013
// Copyright (c) 2013 Samvel Khalatyan. All rights reserved
//
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
//
// (*2.5)   Write a program that strips comments out of a C++ program.
//          That is, read from cin, remove both // comments and /∗ ∗/
//          comments, and write the result to cout. Do not worry about
//          making the layout of the output look nice (that would be
//          another, and much harder, exercise). Do not worry about
//          incorrect programs. Beware of //, /∗, and ∗/ in comments,
//          strings, and character constants.

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "interface/10.7.h"

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::runtime_error;
using std::string;

int main(int argc, char *argv[])
try
{
    if (2 > argc)
        throw invalid_argument(string("usage: ") + argv[0] + " input [input]");

    // process over each input file
    for(int i {1}; argc > i; ++i)
    {
        cout << "-- file " << argv[i] << endl;

        ifstream is(argv[i]);
        if (!is)
        {
            cerr << "failed to open file " << argv[i] << endl;

            continue;
        }

        ex_10_7::Highlighter processor;
        processor.run(is, cout);
    }
}
catch(const std::exception &e)
{
    cerr << e.what() << endl;
}
