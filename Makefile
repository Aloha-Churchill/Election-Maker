CXX = g++
CXXFLAGS	= -std=c++17 -Wall

all: election

clean:
	rm election *.o

election: main.cpp ElectoralMap.o District.o Election.o Candidate.o
	$(CXX) $(CXXFLAGS) main.cpp ElectoralMap.o District.o Election.o Candidate.o -o election

ElectoralMap.o: ElectoralMap.cpp
	$(CXX) $(CXXFLAGS) -c ElectoralMap.cpp

District.o: District.cpp
	$(CXX) $(CXXFLAGS) -c District.cpp

Election.o: Election.cpp
	$(CXX) $(CXXFLAGS) -c Election.cpp

Candidate.o: Candidate.cpp
	$(CXX) $(CXXFLAGS) -c Candidate.cpp