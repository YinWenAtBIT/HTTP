C=gcc
CXX=g++
CFLAGS= -g -D_DEBUG -fPIC -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Winvalid-pch -fms-extensions -Wall -MMD
CPPFLAGS=$(CFLAGS) -Woverloaded-virtual -Wsign-promo -fno-gnu-keywords -std=c++11


OBJS :=tiny.o csapp.o  

LIBPATH=/usr/bin

LIB=-lpthread

TARGET=tinyHTTP

all : $(TARGET) 

check : $(TARGET)
		./$^

$(TARGET) : $(OBJS)
		$(CXX) $(CPPFLAGS) -o $@ $(OBJS) -L$(LIBPATH) $(LIB)		



tiny.o: tiny.cpp
		$(CXX) $(CPPFLAGS) -c $<  -L$(LIBPATH) $(LIB)		


clean:
		rm -rf *.o *.d $(OBJS) $(DEPS) $(TARGET) 

