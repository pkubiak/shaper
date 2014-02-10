CXXFLAGS = -std=c++11 -Wall -Wshadow -Wextra -Wno-unused-parameter
INCLUDES = -I/usr/include/boost

SOURCES = $(wildcard *.cpp)
FILES = $(patsubst %.cpp,%.o,$(SOURCES))
SUBDIRS = lib
TARGET = 
OBJS = $(patsubst %.cpp,%.o,$(shell find . -name \*.cpp -print))

.PHONY: subdirs $(SUBDIRS)

all: $(FILES) subdirs
	g++ $(CXXFLAGS) $(INCLUDES) $(OBJS) -o shaper

subdirs: $(SUBDIRS)
	
$(SUBDIRS):
	$(MAKE) -C $@ $(TARGET)

%.o: %.cpp
	g++ -c $(CXXFLAGS) $^ -o $@
	

#Cleaning 
clean: TARGET = clean
clean: $(SUBDIRS)
	rm -f *.o
	rm -f ./shaper



