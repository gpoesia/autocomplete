#include <algorithm>
#include <iostream>
#include "stringlist.h"

using namespace std;

StringList::StringList()
{
}

void
StringList::addString(const string &s)
{
    strings_.push_back(s);
}

void
StringList::sort()
{
    std::sort(strings_.begin(), strings_.end());
}

vector<string>
StringList::possibleCompletions(const string &prefix)
{
    auto first = lower_bound(strings_.begin(), strings_.end(), prefix);
    auto last = upper_bound(strings_.begin(), strings_.end(), prefix,
            [](const string &a, const string &b) {
                return a < b.substr(0, a.size());
            });

    return vector<string>(first, last);
}
