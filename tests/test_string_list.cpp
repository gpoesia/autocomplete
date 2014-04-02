#include <gtest/gtest.h>
#include "../stringlist.h"

#include <algorithm>
#include <vector>

using namespace std;

TEST(StringListTest, Autocompletion)
{
    StringList sl;
    sl.addString("test");
    sl.addString("tests");
    sl.addString("test1");
    sl.addString("test2");
    sl.addString("different test");
    sl.addString("yet another different test");
    sl.addString("total: many tests");

    auto result = sl.possibleCompletions("t");

    vector<string> answer {"test", "tests", "test1", "test2", "total: many tests"};
    sort(answer.begin(), answer.end());

    EXPECT_EQ(answer, result);

    result = sl.possibleCompletions("test");

    answer = vector<string> { "test", "tests", "test1", "test2"};
    sort(answer.begin(), answer.end());

    EXPECT_EQ(answer, result);

    result = sl.possibleCompletions("d");
    EXPECT_EQ((vector<string> { "different test" }),
            result);
}
