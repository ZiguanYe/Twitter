#include "cmdhandler.h"
#include "util.h"
using namespace std;


//handle QUIT
QuitHandler::QuitHandler() = default;

QuitHandler::QuitHandler(Handler* next) : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
	if (cmd == "QUIT")
    {
        return true;
    }
    else
    {
        return false;
    }

}

Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, std::istream& instr) const
{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}



//handle AND
AndHandler::AndHandler() = default;

AndHandler::AndHandler(Handler *next) : Handler(next)
{

}

bool AndHandler::canHandle(const std::string &cmd) const
{
    if (cmd == "AND")
    {
        return true;
    }
    else
    {
        return false;
    }
}


Handler::HANDLER_STATUS_T AndHandler::process(TwitEng *eng, istream &instr) const
{
    vector<string> hashVec;
    string hashtag;
    while (instr >> hashtag)
    {
        hashVec.push_back(hashtag);
    }
    
    vector<Tweet*> result = eng->search(hashVec, 0);
    displayHits(result);
    return HANDLER_OK;
}


//handle OR
OrHandler::OrHandler() = default;

OrHandler::OrHandler(Handler *next) : Handler(next)
{

}

bool OrHandler::canHandle(const string &cmd) const
{
    if (cmd == "OR")
    {
        return true;
    }
    else
    {
        return false;
    }
}

Handler::HANDLER_STATUS_T OrHandler::process(TwitEng *eng, istream &instr) const
{
    vector<string> hashVec;
    string hashtag;
    while (instr >> hashtag)
    {
        hashVec.push_back(hashtag);
    }
    
    vector<Tweet*> result = eng->search(hashVec, 1);
    displayHits(result);
    return HANDLER_OK;
}


//handle TWEET
TweetHandler::TweetHandler() = default;

TweetHandler::TweetHandler(Handler *next) : Handler(next)
{

}

bool TweetHandler::canHandle(const string &cmd) const
{
    if (cmd == "TWEET")
    {
        return true;
    }
    else
    {
        return false;
    }
}

Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng *eng, istream &instr) const
{
    string username, text;
    DateTime date = DateTime();
    
    instr >> username;
    if (instr.fail() || eng->validUser(username) == false)
    {
        return HANDLER_ERROR;
    }
    
    getline(instr, text);
    if (instr.fail())
    {
        text = "";
    }
    trim(text);
    
    eng->addTweet(username, date, text);
    return HANDLER_OK;
}
