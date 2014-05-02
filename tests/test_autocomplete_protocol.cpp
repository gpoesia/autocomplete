#include <gtest/gtest.h>
#include "../autocompleteprotocol.h"

#include <string>
#include <vector>

using namespace std;

TEST(ProtocolTest, Query)
{
    auto queries = {"word", "two words", ";$asdf##$"};

    for (auto q : queries) {
        EXPECT_EQ(q, DecodeQuery(FormatQuery(q)));
    }
}

TEST(ProtocolTest, Response)
{
    vector<vector<string>> responses = {
        { "first", "foremost", "final", "finally" },
        {},
        {"first$", ";second", ";;;third$$;"}
    };

    for (auto r : responses) {
        EXPECT_EQ(r, DecodeResponse(FormatResponse(r)));
    }
}

TEST(ProtocolTest, MessageTermination)
{
    vector<string> queries {"first.", "asdkçlf", "powijeriáéí;;$$;$;4", "$"};

    vector<vector<string>> responses = {
        { "first", "foremost", "final", "finally" },
        {},
        {"first$", ";second", ";;;third$$;"}
    };

    for (auto q : queries) {
        string message = FormatQuery(q);
        EXPECT_EQ(message.size(), MessageLength(message));
        EXPECT_EQ(message.size(), MessageLength(message + message));
        EXPECT_EQ(message.size(), MessageLength(message + message + q + ";;$"));

        for (size_t i = 0; i < message.size(); i++)
            EXPECT_EQ(0, MessageLength(message.substr(0, i)));
    }
}
