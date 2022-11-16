CXX=g++
CXXFLAGS=-g -Wall -std=c++11

all: twitter

# Finish this target by adding other dependencies and 
# the action to compile the final twitter executable
twitter: twitter.o user.o cmdhandler.o handler.o tweet.o util.o datetime.o twiteng.o
	$(CXX) $(CXXFLAGS) twitter.o user.o cmdhandler.o handler.o tweet.o util.o datetime.o twiteng.o -o twitter

# Complete
twitter.o: twitter.cpp twiteng.h cmdhandler.h util.h
	$(CXX) $(CXXFLAGS) -c twitter.cpp -o twitter.o

# Add other rules/targets
user.o: user.cpp user.h tweet.h
	$(CXX) $(CXXFLAGS) -c user.cpp -o user.o

cmdhandler.o: cmdhandler.cpp cmdhandler.h util.h
	$(CXX) $(CXXFLAGS) -c cmdhandler.cpp -o cmdhandler.o

handler.o: handler.cpp handler.h
	$(CXX) $(CXXFLAGS) -c handler.cpp -o handler.o

tweet.o: tweet.cpp tweet.h user.h
	$(CXX) $(CXXFLAGS) -c tweet.cpp -o tweet.o

util.o: util.cpp util.h
	$(CXX) $(CXXFLAGS) -c util.cpp -o util.o

twiteng.o: twiteng.h tweet.h util.h
	$(CXX) $(CXXFLAGS) -c twiteng.cpp -o twiteng.o

datetime.o: datetime.h
	$(CXX) $(CXXFLAGS) -c datetime.cpp -o datetime.o

clean:
	rm -f *.o twitter *.feed
