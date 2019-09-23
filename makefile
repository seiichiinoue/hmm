CC = clang++
CCFLAGS = -std=c++11 -stdlib=libc++
BOOST = /usr/local/Cellar/boost/1.71.0
INCLUDE = -I/usr/local/include/ -L/usr/local/lib
LDFLAGS = -lglog -lgflags

hmm:
	$(CC) -o hmm $(CCFLAGS) src/hmm.cpp src/model.cpp $(INCLUDE) $(LDFLAGS)

test:
	$(CC) $(CCFLAGS) src/hmm.cpp src/test.cpp $(INCLUDE) $(LDFLAGS)

clean:
	rm -f hmm

.PHONY: test clean