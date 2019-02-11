# lua-cpp-jwt
A small wrapper arround [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) (to decode and verify jwt).

### Why

We couldn't find any jwt verification library for lua for the algorithm we use, and we chose [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) because it supports many verification algorithms, besides being a single header library with an elegant api.

### Note
Currently, only decoding/verification are implemented (also, tailored for a specific algorithm).

As noted above, this wrapper is created for a speicifc use case!

### Building
A simple `Makefile` is created to build the lua library using [luarocks](https://luarocks.org/).
Doing `luarocks make` would build and copy shared library and lua files to correct destinations.

### Installation
The easiest way to use this library is to install via [luarocks](https://luarocks.org/)

`luarocks install --server abom/cppjwt`
