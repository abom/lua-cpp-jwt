CC = g++
SRC = cppjwt/cppjwt.cpp
INCLUDE = -Icppjwt -Ivendor/jwt-cpp/include -I$(LUA_INCDIR)
LIBS_PATH = -L$(LUA_LIBDIR)
LIBS = -lcrypto -lpthread -lstdc++
FLAGS = -std=c++11 -Wall $(LIBFLAG) $(CFLAGS)
OUT = libcppjwt.so

all: build

build:
	$(CC) $(SRC) $(FLAGS) $(INCLUDE) $(LIBS_PATH) $(LIBS) -o $(OUT)

clean:
	rm *.so

install:
	cp $(OUT) $(INST_LIBDIR)
	cp -a lib/cpp $(INST_LUADIR)
