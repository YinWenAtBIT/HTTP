C=gcc
CXX=g++
CFLAGS= -g -D_DEBUG -fPIC -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare -Winvalid-pch -fms-extensions -Wall -MMD
CPPFLAGS=$(CFLAGS) -Woverloaded-virtual -Wsign-promo -fno-gnu-keywords -std=c++11


SRC_DIR = ./src
BUILD_DIR = ./Build

MAIN_DIR = ./Main

OBJS := csapp.o Channel.o Epoll.o  Echo.o helper.o HTTP.o HTTP_base.o HTTP_Request.o HTTP_Response.o



LIBPATH=/usr/bin

LIB=-lpthread #-fpermissive

TARGET=my_echo my_HTTP



all : $(TARGET) 

check : $(TARGET)
		./$^

my_echo : $(OBJS) echo_server.o
		$(CXX) $(CPPFLAGS) -o $(MAIN_DIR)/$@ $(MAIN_DIR)/echo_server.o $(BUILD_DIR)/*.o -L$(LIBPATH) $(LIB)		

my_HTTP : $(OBJS) http_server.o
		$(CXX) $(CPPFLAGS) -o $(MAIN_DIR)/$@ $(MAIN_DIR)/http_server.o $(BUILD_DIR)/*.o -L$(LIBPATH) $(LIB)		

echo_server.o: 
		$(CXX) $(CPPFLAGS) -c $(SRC_DIR)/echo_server.cpp -o $(MAIN_DIR)/$@ -L$(LIBPATH)  $(LIB)

http_server.o: 
		$(CXX) $(CPPFLAGS) -c $(SRC_DIR)/http_server.cpp -o $(MAIN_DIR)/$@ -L$(LIBPATH)  $(LIB)


csapp.o:
		$(CXX) $(CPPFLAGS) -c ./Base/csapp.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

Channel.o:
		$(CXX) $(CPPFLAGS) -c ./EventLoop/Channel.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

Epoll.o:
		$(CXX) $(CPPFLAGS) -c ./EventLoop/Epoll.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

Echo.o:
		$(CXX) $(CPPFLAGS) -c ./Protocol/Echo.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

helper.o:
		$(CXX) $(CPPFLAGS) -c ./Base/helper.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

HTTP.o:
		$(CXX) $(CPPFLAGS) -c ./Protocol/HTTP.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

HTTP_base.o:
		$(CXX) $(CPPFLAGS) -c ./Protocol/HTTP_base.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

HTTP_Request.o:
		$(CXX) $(CPPFLAGS) -c ./Protocol/HTTP_Request.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

HTTP_Response.o:
		$(CXX) $(CPPFLAGS) -c ./Protocol/HTTP_Response.cpp -o $(BUILD_DIR)/$@ -L$(LIBPATH)  $(LIB)

clean:
		cd ./Build && rm -rf .o *.d $(OBJS) $(DEPS) $(TARGET) && cd ../Main && rm -rf *.o *.d my*

