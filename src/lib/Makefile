CXXFLAGS = -std=c++11 -Wall -Wshadow -Wextra -Wno-unused-parameter
FLAGS = -DLIBSHAPER_SINGLETON
FLAGS+= $(CXXFLAGS)

SOURCES = $(wildcard *.cpp)
FILES = $(patsubst %.cpp,%.o,$(SOURCES))
SUBDIRS = common singleton
TARGET = 
#OBJS = $(patsubst %.cpp,%.o,$(shell find . -name \*.cpp -print))

.PHONY: subdirs $(SUBDIRS)

all: $(FILES) subdirs
	

subdirs: $(SUBDIRS)
	
$(SUBDIRS):
	$(MAKE) -C $@ $(TARGET)

%.o: %.cpp
	g++ -c $(FLAGS) $^ -o $@
	

#Cleaning 
clean: TARGET = clean
clean: $(SUBDIRS)
	rm -f *.o



