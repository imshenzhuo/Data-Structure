cc = g++
eflags = -std=c++11 -g -O1

files = TestExtendibleTest TestHashtable_link

all: $(files)

TestExtendibleTest: extendible_hash.cpp extendible_hash.h
	$(cc) $(eflags) -o TestExtendibleHash extendible_hash.cpp

TestHashtable_link: link_hash.cpp link_hash.h
	$(cc) $(eflags) -o TestLinkHash link_hash.cpp

clean:
	rm  TestLinkHash TestExtendibleHash

