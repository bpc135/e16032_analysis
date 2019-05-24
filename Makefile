CC=gcc
CXX=g++
CXXLD=g++
CCLD=gcc

CXXFLAGS=-g -I. -I/usr/share/tcl8.5 -pthread -fPIC -I/home/bpc135/ringblockdealer/ParallelAnalysis/main/threadfit -I/usr/opt/ddas/include
CCFLAGS=$(CXXFLAGS)

LDFLAGS= -L/usr/lib -lpthread -lrt

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTLDFLAGS  := $(shell root-config --ldflags)

LIB_DIR=$(PWD)
#LIB_DIR=/user/e16032/RootAnalysis_Scripted

LIBS  = -lm $(ROOTGLIBS) -L$(LIB_DIR) -I. -lMinuit -Wl,-rpath,$(LIB_DIR) -lParameters-ddas -Wl,-rpath,/usr/opt/ddas/lib -L/usr/opt/ddas/lib -lddaschannel -Wl,-rpath,/home/bpc135/Utilities_root6  -L/home/bpc135/Utilities_root6 -lPixie16Utilities -L/home/bpc135/ringblockdealer/ParallelAnalysis/main/threadfit -Wl,-rpath,/home/bpc135/ringblockdealer/ParallelAnalysis/main/threadfit -lDDASFitClasses 

CXXFLAGS+=$(ROOTCFLAGS)
O_FILES = correlator.o Variables-ddas.o main.o unpacker.o analyzer.o 

all: e16032_analysis

e16032_analysis: $(O_FILES) libParameters-ddas.so 
	$(CXXLD) -o e16032_analysis $(O_FILES) $(LDFLAGS) $(LIBS)

Parameters-ddasDictionary.cxx: Parameters-ddas.h Parameters-ddas_linkdef.h
	rm -f Parameters-ddasDictionary.cc Parameters-ddasDictionary.h
	rootcint -f $@ -c $(CXXFLAGS) -p Parameters-ddas.h Parameters-ddas_linkdef.h

libParameters-ddas.so: Parameters-ddasDictionary.cxx Parameters-ddas.cpp 
	$(CXX) -fPIC -shared -o$@ $(ROOTLDFLAGS) $(CXXFLAGS) -I$(ROOTSYS)/include $^

ddaschannelDictionary.cxx: ddaschannel.h DDASEvent.h LinkDef.h
	rm -f ddaschannelDictionary.cpp ddaschannelDictionary.h
	rootcint -f $@ -c -p ddaschannel.h DDASEvent.h LinkDef.h

ddaschannelDictionary.o: ddaschannelDictionary.cxx
	$(CXX) -c -o $@ $(CXXFLAGS) $^

depend:
	makedepend $(CXXFLAGS) *.cpp *.c 

clean:
	rm -f *.o e16032_analysis libParameters-ddas.so Parameters-ddasDictionary* 
# DO NOT DELETE
