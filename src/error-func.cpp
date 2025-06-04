/*
    * error-func.cpp
    * This source file implements utility functions for error checking and input validation.
    *
    * Contributors: Kade Miller
*/

#include "error-func.h"

bool check_range(int lower, int upper, int x)
{
	return x >= lower && x <= upper;
}

int equality_test(int x, int y)
{
	return x == y ? 0 : 
		   x > y ? 1 : 
		   -1;
}

bool is_int(std::string num)
{
    if (num.length() == 0) // If the string is empty, it can't be a number.
        return false;

    // loop through each character, than check if it's between 48 and 57 on the ascii table.
    for (int i = 0; i < num.length(); i++)
    {
        char c = num[i];
        if ((int)c < 48 || (int)c > 57)
            return false;
    }
    return true;
}

bool is_float(std::string f)
{
    if (f.length() == 0) // If the string is empty, it can't be a float.
        return false;

    int periodCount = 0; // Count the number of periods in the string

    // loop through each character, than check if it's between 48 and 57 on the ascii table.
    for (int i = 0; i < f.length(); i++)
    {
        char c = f[i];

        if ((int)c == 46) // period
        {
            if (i <= 0) // If the period is the first character, that isn't valid here.
                return false;
            if (i == f.length() - 1) // If the period is the last character, that isn't valid here.
                return false;
            periodCount++; // increment the period count
            if (periodCount > 1) // If there is more than one period, that isn't valid here.
                return false;
            continue; // continue with the next itteration.
        }

        if ((int)c < 48 || (int)c > 57)
            return false;
    }
    return true;
}

void string_to_int(std::string num, int& v)
{
	if (is_int(num))
		v = std::stoi(num);
}

void swap(int& n, int& m)
{
	// could just use std::swap, but I don't think you want that.
	int temp = n;
	n = m;
	m = temp;
}

int word_count(std::string sentence)
{
	if (sentence.length() == 0)
        return 0;

	int words = 0;
	for (int i = 0; i < sentence.length(); i++)
    {
		if (i != 0 && (int)sentence[i] == 32 && (int)sentence[i-1] != 32) // a space, and the last character isn't a space.
			words++;
	}

	return words + 1;
}