/*
    * error-func.h
    * This header file defines utility functions for error checking and input validation.
    * 
    * Contributors: Kade Miller
*/

#include <string>
#include <iostream>

bool check_range(int lower, int upper, int x);
int equality_test(int x, int y);
bool is_int(std::string num);
bool is_float(std::string num);
void string_to_int(std::string num, int& out);
void swap(int& n, int& m);
int word_count(std::string sentence);