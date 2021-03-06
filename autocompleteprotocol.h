#ifndef AUTOCOMPLETE_PROTOCOL_H
#define AUTOCOMPLETE_PROTOCOL_H

#include <string>
#include <vector>

/// Formats a query string to be sent to an autocomplete server.
std::string FormatQuery(const std::string &query);

/// Returns the query string corresponding to a given formatted query message.
/// If this string is empty, the client wants to close its connection to the
/// server.
std::string DecodeQuery(const std::string &formattedQuery);

/// Returns a message that closes the connection to the server
std::string FormatCloseConnection();

/// Formats a list of possible completions to be sent to an autocomplete client.
std::string FormatResponse(const std::vector<std::string> &response);

/// Returns the list of possible completions corresponding to a
/// given formatted response.
std::vector<std::string> DecodeResponse(const std::string &formattedResponse);

/// Returns the length of the first complete encoded message in the string
/// (i.e. a formatted query or reponse).
/// If the string has no complete message, returns 0.
size_t MessageLength(const std::string &message);

#endif
