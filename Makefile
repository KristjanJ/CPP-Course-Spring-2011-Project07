# To compile, run 'make' in the same directory as this file

##################
# Boost location #
##################

# Use the directory that *contains* the boost directory
BOOST_HEADERS = C:\Users\Kristjan\Desktop\Kristjan\University\C++\boost\boost_1_41_0
BOOST_LIBRARIES = C:\Users\Kristjan\Desktop\Kristjan\University\C++\boost\boost-win32-gcc4x\lib
# The flavour is the suffix in the library name
# For example: if the library is libboost_filesystem-mt.a, then FLAVOUR is mt
BOOST_FLAVOUR = mgw45-mt-1_41

# The -g flag adds debugging support
CFLAGS = -Wall -g -Iinclude -I$(BOOST_HEADERS)
LFLAGS = -L$(BOOST_LIBRARIES) -lboost_program_options-$(BOOST_FLAVOUR) -lboost_filesystem-$(BOOST_FLAVOUR) -lboost_thread-$(BOOST_FLAVOUR) -lboost_system-$(BOOST_FLAVOUR)

#########################
# Settings for the apps # 
#########################

FINDTEXTOBJS = obj/findtext.o
FINDTEXTAPP = bin/findtext

SORTFILESOBJS = obj/sortfiles.o
SORTFILESAPP = bin/sortfiles

PHILOSOPHEROBJS = obj/philosophers.o
PHILOSOPHERAPP = bin/philosophers

# This is the first target. It will be built when you run 'make' or 'make all'
all: $(FINDTEXTAPP) $(PHILOSOPHERAPP) $(SORTFILESAPP)

# Rules for linking the test apps
$(FINDTEXTAPP): $(FINDTEXTOBJS)
	$(CXX) $(FINDTEXTOBJS) -o $(FINDTEXTAPP) $(LFLAGS)

$(PHILOSOPHERAPP): $(PHILOSOPHEROBJS)
	$(CXX) $(PHILOSOPHEROBJS) -o $(PHILOSOPHERAPP) $(LFLAGS)

$(SORTFILESAPP): $(SORTFILESOBJS)
	$(CXX) $(SORTFILESOBJS) -o $(SORTFILESAPP) $(LFLAGS)

# Compile each source file of the apps
obj/philosophers.o: src/philosophers.cpp
	$(CXX) $(CFLAGS) -c src/philosophers.cpp -o obj/philosophers.o

obj/findtext.o: src/findtext.cpp
	$(CXX) $(CFLAGS) -c src/findtext.cpp -o obj/findtext.o

obj/sortfiles.o: src/sortfiles.cpp
	$(CXX) $(CFLAGS) -c src/sortfiles.cpp -o obj/sortfiles.o

clean: 
	rm -rf $(FINDTEXTAPP) $(FINDTEXTOBJS) $(PHILOSOPHERAPP) $(PHILOSOPHEROBJS) $(SORTFILESAPP) $(SORTFILESOBJS) docs/html

doc: 
	doxygen
