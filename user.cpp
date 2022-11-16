
#include <algorithm>
#include "user.h"
#include "tweet.h"

using namespace std;

User::User(string name) : username(name)
{
}

User::~User()
{
    for (list<Tweet *>::iterator It=tweet.begin(); It!=tweet.end(); ++It)
    {
        delete *It;
    }
}

std::string User::name() const
{
    return username;
}

std::set<User *> User::followers() const
{
    return foerSet;
}

std::set<User *> User::following() const
{
    return foingSet;
}

std::list<Tweet *> User::tweets() const
{
    return tweet;
}

void User::addFollower(User *u)
{
    foerSet.insert(u);
}

void User::addFollowing(User *u)
{
    foingSet.insert(u);
}

void User::addTweet(Tweet *t)
{
    tweet.push_back(t);
}

std::vector<Tweet *> User::getFeed()
{
    vector<Tweet *> twVec;
    
    for (list<Tweet *>::iterator It = tweet.begin(); It != tweet.end(); ++It)
    {
        twVec.push_back(*It);
    }
    
    for (set<User *>::iterator It = foingSet.begin(); It != foingSet.end(); ++It)
    {
        list<Tweet *> tweets = (*It)->tweets();
        for (list<Tweet *>::iterator I2 = tweets.begin(); I2 != tweets.end(); ++I2)
        {
        	twVec.push_back(*I2);
        }
    }
    
    sort(twVec.begin(), twVec.end(), TweetComp());
    return twVec;
}
