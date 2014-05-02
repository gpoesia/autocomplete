#include "autocompleteprotocol.h"

using namespace std;

/*
    The query/response protocol is as follows:

    Query:
        word$

    Response:
        word;word;word; ... ;word$

    The protocol uses byte stuffing to allow the characters '$' and ';' to
    appear inside the words. The escape character is the backslash, '\'.
    Thus, '\' is substituted by "\\", '$' becomes "\$" and ';' turns into "\;".

   */

const char END_CHARACTER = '$';
const char SEPARATOR_CHARACTER = ';';
const char ESCAPE_CHARACTER = '\\';
const string ESCAPED_END_CHARACTER = "\\$";
const string ESCAPED_SEPARATOR_CHARACTER = "\\;";
const string ESCAPED_ESCAPE_CHARACTER = "\\\\";

namespace
{
    // Substitutes '\' for '\\' and '$' for '\$'
    string escape(const string &s)
    {
        string escapedS;
        escapedS.reserve(s.size());

        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] == END_CHARACTER)
                escapedS += ESCAPED_END_CHARACTER;
            else if (s[i] == ESCAPE_CHARACTER)
                escapedS += ESCAPED_ESCAPE_CHARACTER;
            else if (s[i] == SEPARATOR_CHARACTER)
                escapedS += ESCAPED_SEPARATOR_CHARACTER;
            else
                escapedS += s[i];
        }

        return escapedS;
    }

    // Reverses escape(). Assumes the string is valid.
    string unescape(const string &s)
    {
        string originalS;

        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] == ESCAPE_CHARACTER) {
                i++;
            }

            originalS += s[i];
        }

        return originalS;
    }
}


/// Formats a query string to be sent to an autocomplete server.
std::string FormatQuery(const std::string &query)
{
    return escape(query) + END_CHARACTER;
}

std::string DecodeQuery(const std::string &message)
{
    return unescape(message.substr(0, message.size() - 1));
}

std::string FormatResponse(const vector<string> &response)
{
    string message;

    for (size_t i = 0; i < response.size(); i++) {
        if (i)
            message += SEPARATOR_CHARACTER;
        message += escape(response[i]);
    }

    return message + END_CHARACTER;
}

vector<string> DecodeResponse(const string &message)
{
    vector<string> response;
    string currentElement;

    for (size_t i = 0; i < message.size(); i++) {
        if (i + 1 == message.size() ||
                (message[i] == SEPARATOR_CHARACTER &&
                 (i == 0 || message[i-1] != ESCAPE_CHARACTER))) {

            if (i > 0)
                response.push_back(unescape(currentElement));
            currentElement = string();
        } else {
            currentElement += message[i];
        }
    }

    return response;
}

size_t MessageLength(const string &message)
{
    for (unsigned i = 0; i < message.size(); i++) {
        if (message[i] == END_CHARACTER &&
                (i == 0 || message[i-1] != ESCAPE_CHARACTER))
            return i + 1;
    }

    return 0;
}
