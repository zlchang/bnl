SOURCES = Test.cxx
HEADERS = MyDef.h Test.h\
	  MyDefLinkDef.h
OBJECTS = MyDef.so
DICT = MyDefDict.cxx

LD=g++

CXXFLAGS = $(shell root-config --cflags)
#LDFLAGS = $(shell root-config --ldflags)

$(OBJECTS) : $(DICT) $(SOURCES)
	$(LD) -shared $(CXXFLAGS) $(DICT) $(SOURCES) -o $(OBJECTS)

$(DICT): $(HEADERS)
	rootcint -f $(DICT) -c $(HEADERS) 
