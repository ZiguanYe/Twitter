#include <string>
#include <sstream>
#include "user.h"
#include "tweet.h"

using namespace std;

Tweet::Tweet()
{
    person = nullptr;
}

Tweet::Tweet(User *user, const DateTime &time, const string &text) : person(user), datetime(time), twtext(text)
{
    string str;
    stringstream ss(text);
    while (ss >> str)
    {
        if (str[0] == '#')
        {
            //erase the hashtag sign
            str.erase(0, 1);
            hashtag.insert(str);
        }
    }
}

DateTime const &Tweet::time() const
{
    return datetime;
}

std::string const &Tweet::text() const
{
    return twtext;
}

std::set<std::string> Tweet::hashTags() const
{
    return hashtag;
}

bool Tweet::operator<(const Tweet &other) const
{
    if (datetime < other.datetime)
    {
        return true;
    }
    else
    {
        return false;
    }
}

std::ostream &operator<<(ostream &os, const Tweet &t)
{
    os << t.time() << " " << t.user()->name() << " " << t.text();
    return os;
}

User *Tweet::user() const
{
    return person;
}
