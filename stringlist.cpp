#include <algorithm>
#include <iostream>
#include "stringlist.h"

using namespace std;

StringList::StringList()
    : sorted_(false)
{
}

void
StringList::addString(const string &s)
{
    strings_.push_back(s);
    sorted_ = false;
}

vector<string>
StringList::possibleCompletions(const string &prefix)
{
    if (!sorted_) {
        sort(strings_.begin(), strings_.end());
        sorted_ = true;
    }

    auto first = lower_bound(strings_.begin(), strings_.end(), prefix);
    auto last = upper_bound(strings_.begin(), strings_.end(), prefix,
            [](const string &a, const string &b) {
                return a < b.substr(0, a.size());
            });

    return vector<string>(first, last);
}
