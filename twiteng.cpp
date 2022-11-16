t#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "twiteng.h"
// Add other header files
#include "tweet.h"
#include "util.h"
#include "user.h"


using namespace std;


// Update as needed
TwitEng::TwitEng() = default;

// Update as needed
TwitEng::~TwitEng()
{
    for (map<string, User*>::iterator It=userMap.begin(); It!=userMap.end(); ++It)
    {
        //note that the value of the map is User
        User* user = It->second;
        list<Tweet*> tweets = user->tweets();
        for (list<Tweet*>::iterator twIt=tweets.begin(); twIt!=tweets.end(); ++twIt)
        {
            delete *twIt;
        }
        delete user;
    }
}

// Complete this function
bool TwitEng::parse(char* filename)
{
  ifstream ifile(filename);
  if (ifile.fail()) {
    cerr << "Unable to open " << filename << endl;
    return true;
  }
  int numUsers;
  ifile >> numUsers;
  if (ifile.fail()) {
    cerr << "Unable to read number of users " << endl;
    ifile.close();
    return true;
  }
  // Finish the parsing of the input file. Return false if successful,
  // true otherwise.
    
    //read users and their following
    string line;
    //skip the first line since it is numUsers
    getline(ifile, line);
    
    for (int i=0; i<numUsers; i++)
    {
        //readline
        getline(ifile, line);
        stringstream ss(line);
        
        string tmpname;
        //stringstream the user name / not the following list
        ss >> tmpname;
        //case: the user has not appeared before
        if (!validUser(tmpname))
        {
            //allocate a new user if it is a new one
            User *tmpUser = new User(tmpname);
            //add the user to userMap
            userMap.insert(make_pair(tmpname, tmpUser));
        }
        //do nothing in the other case, no else here
        
        //create new string to hold the following usernames
        string username;
        //stringstream the following list
        while (ss >> username)
        {
            //case: the following user has appeared before
            if (validUser(username))
            {
                //add following and follower
                userMap[tmpname]->addFollowing(userMap[username]);
                userMap[username]->addFollower(userMap[tmpname]);
            }
            //case: the following user is a new user
            else
            {
                //allocate a new user
                User *newUser = new User(username);
                //add a new user to userMap
                userMap.insert(make_pair(username, newUser));
                //add following and follower
                userMap[tmpname]->addFollowing(newUser);
                newUser->addFollower(userMap[tmpname]);
            }
        }
        
    }
    
    //read the tweets
    string twtext;
    //the loop start from first tweet to the end of file, thus while loop
    while (getline(ifile, twtext))
    {
        //skip if empty
        if (!twtext.empty())
        {
            DateTime tweetDate;
            string username, tweet;
            //stringstream the info
            stringstream ss(twtext);
            ss >> tweetDate;
            ss >> username;
            
            if (ss.fail() || !validUser(username))
            {
                return true;
            }
            //note that getline need to be called from here, or ss would break the line
            getline(ss, tweet);
            //clear the leading or trailing spaces
            trim(tweet);
            addTweet(username, tweetDate, tweet);
        }
    }
    
    return false;
}

// Implement other member functions below

void TwitEng::addTweet(const std::string& username, const DateTime& time, const std::string& text)
{
    //create new user, username shall be valid already
    User *user = userMap[username];
    //allocate a new tweet with given info
    Tweet *tweet = new Tweet(user, time, text);
    
    //put tweet into the hasMap
    //create new hashtag set to hold all #s of the tweet
    set<string> hashtag = tweet->hashTags();
    //iterate through hashtags
    for (set<string>::iterator It=hashtag.begin(); It!=hashtag.end(); ++It)
    {
        string hash = *It;

        convLower(hash);
        //case: the hashtag is already in the hashMap
        if (hashMap.find(hash) == hashMap.end())
        {
            set<Tweet *> temp;
            temp.insert(tweet);
            hashMap.insert(make_pair(hash, temp));
        }
        else
        {
            hashMap[hash].insert(tweet);
        }
    }
    
    //call the addTweet function from user to pushback the tweet to tweet list
    user->addTweet(tweet);
}


std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy)
{
    //create new vec to store searching results
    vector<Tweet *> result = {};
    //if no term included just return
    if (terms.size()==0)
    {
        return result;
    }
    
    //unify all characters to be lowercase for searching
    //note: can use either convLower or convUpper, but one func would be defined but unused in the entire hw2
    //do not know if that is a problem
    for (unsigned int i=0; i<terms.size(); i++)
    {
        convLower(terms[i]);
    }
    
    //case: there is only one term, no OR or AND
    if (terms.size() == 1)
    {
        //found sth
        if (hashMap.find(terms[0]) != hashMap.end())
        {
            set<Tweet *> twSet = hashMap[terms[0]];
            for (set<Tweet *>::iterator It=twSet.begin(); It!=twSet.end(); ++It)
            {
                result.push_back(*It);
            }
            return result;
        }
        //cant find anything, just return
        else
        {
            return result;
        }
    }
    //case: more than 1 term and strategy is OR
    else if (strategy == 1)
    {
        //create temp to store temporary set, should not pushback to result directly
        vector<set<Tweet *>> temp;
        
        for (size_t i=0; i<terms.size(); i++)
        {
            string key = terms[i];
            //continue would be ok since the strategy is OR
            if (hashMap.find(key) != hashMap.end())
            {
                temp.push_back(hashMap[key]);
            }
            else
            {
                continue;
            }
        }
        
        set<Tweet *> tmpSet;
        //call the setUnion fuction to exclude repeated tweets and address OR strategy
        tmpSet = setUnion(temp);
        
        for (set<Tweet *>::iterator It=tmpSet.begin(); It!=tmpSet.end(); ++It)
        {
            result.push_back(*It);
        }
        
        return result;
        
    }
    //case: more than 1 term and strategy is AND
    else if (strategy == 0)
    {
        //create temp to store temporary set, should not pushback to result directly
        vector<set<Tweet *>> temp;
        
        for (size_t i=0; i<terms.size(); i++)
        {
            string key = terms[i];
            //if the key is found
            if (hashMap.find(key) != hashMap.end())
            {
                temp.push_back(hashMap[key]);
            }
            //return empty result directly if AND strategy fails
            else
            {
                return result;
            }
        }
        
        set<Tweet *> tempSet;
        //call the setCommon fuction to hold the repeated tweets
        tempSet = setCommon(temp);
        
        for (set<Tweet *>::iterator It=tempSet.begin(); It!=tempSet.end(); ++It)
        {
            result.push_back(*It);
        }
        
        return result;
    }
    return result;
}

void TwitEng::dumpFeeds()
{
    for (map<string, User*>::iterator It=userMap.begin(); It!=userMap.end(); ++It)
    {
        ofstream file;
        file.open(It->first + ".feed");
        file << It->first + "\n";
        
        vector<Tweet*> feed = It->second->getFeed();
        for (vector<Tweet *>::iterator I2=feed.begin(); I2!=feed.end(); ++I2)
        {
            file << *(*I2) << endl;
        }
        file.close();
    }
}

bool TwitEng::validUser(const std::string& name) const
{
    if (userMap.find(name) == userMap.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

std::set<Tweet*> TwitEng::setCommon(std::vector<std::set<Tweet*>> twVec)
{
    set<Tweet *> result;
    
    //base case: only two sets in the vector
    if (twVec.size() == 2)
    {
        for (set<Tweet *>::iterator It=twVec[0].begin(); It!=twVec[0].end(); ++It)
        {
        	if (twVec[1].find(*It) != twVec[1].end())
            {
                result.insert(*It);
            }
        }
    }
    else
    {
        //recursive call
        vector<set<Tweet*>> temp;
        temp.push_back(twVec[twVec.size() - 1]);
        twVec.pop_back();
        temp.push_back(setCommon(twVec));
        result = setCommon(temp);
    }
    return result;
}

std::set<Tweet*> TwitEng::setUnion(std::vector<std::set<Tweet*>> twVec)
{
    set<Tweet *> result;
    
    if (twVec.size() == 2)
    {
        //push everything in twVec[0]
        for (set<Tweet *>::iterator It=twVec[0].begin(); It!=twVec[0].end(); ++It)
        {
            result.insert(*It);
        }
        for (set<Tweet *>::iterator I2=twVec[1].begin(); I2!=twVec[1].end(); ++I2)
        {
            //do not insert same tweet, only insert those not in twVec[0]
            if(result.find(*I2) == result.end())
            {
                result.insert(*I2);
            }
        }
    }
    else
    {
        vector<set<Tweet*>> temp;
        temp.push_back(twVec[twVec.size() - 1]);
        twVec.pop_back();
        temp.push_back(setUnion(twVec));
        result = setUnion(temp);
    }
    return result;
}
