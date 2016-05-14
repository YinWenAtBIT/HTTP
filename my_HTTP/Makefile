C=gcc
CXX=g++
CFLAGS= -g -D_DEBUG -fPIC -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Winvalid-pch -fms-extensions -Wall -MMD
CPPFLAGS=$(CFLAGS) -Woverloaded-virtual -Wsign-promo -fno-gnu-keywords -std=c++11


SRC_DIR = ./src
BUILD_DIR = ./Build

OBJS :=echo_server.o csapp.o Channel.o Epoll.o  

LIBPATH=/usr/bin

LIB=-lpthread #-fpermissive

TARGET=my_HTTP

all : $(TARGET) 

check : $(TARGET)
		./$^

$(TARGET) : $(OBJS)
		$(CXX) $(CPPFLAGS) -o $(BUILD_DIR)/$@ $(BUILD_DIR)/*.o  -L$(LIBPATH) $(LIB)		



echo_server.o: 
		$(CXX) $(CPPFLAGS) -c $(SRC_DIR)/echo_server.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

csapp.o:
		$(CXX) $(CPPFLAGS) -c ./Base/csapp.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

Channel.o:
		$(CXX) $(CPPFLAGS) -c ./EventLoop/Channel.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

Epoll.o:
		$(CXX) $(CPPFLAGS) -c ./EventLoop/Epoll.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

clean:
		rm -rf *.o *.d $(OBJS) $(DEPS) $(TARGET) 

