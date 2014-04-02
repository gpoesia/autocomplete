#ifndef STRINGLIST_H
#define STRINGLIST_H

#include <string>
#include <vector>

class StringList
{
    public:
        StringList();

        /// Adds a new string to the string list.
        void addString(const std::string &newString);

        /// Returns all the strings in the list that begin with a given prefix.
        std::vector<std::string> possibleCompletions(const std::string &prefix);

    private:
        std::vector<std::string> strings_;
        bool sorted_;
};

#endif
