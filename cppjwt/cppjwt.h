#ifndef __CPPJWT__
#define __CPPJWT__

#include <lua.hpp>

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/picojson.h>

class unsupported_algorithm : std::exception {
    public:
        unsupported_algorithm(const char* alg_name) : algorithm_name("") {
            this->algorithm_name = alg_name;
        }

        const char* what() const throw() {
            std::string err("unsupported algorithm: " + algorithm_name);
            return strdup(err.c_str());
        }

    private:
        std::string algorithm_name;
};

typedef jwt::verifier<jwt::default_clock> Verifier;

template<typename... AlgArgs>
void allow_algorithm_by_name(Verifier&, const std::string&, AlgArgs...);

extern "C" {
    char* decode(const char*);
    int verify(const char*, const char*);

    static int lua_jwt_decode(lua_State*);
    static int lua_jwt_verify(lua_State*);
    static const struct luaL_Reg cppjwt[] = {
      {"decode", lua_jwt_decode},
      {"verify", lua_jwt_verify},
      {NULL, NULL}
    };
    int luaopen_libcppjwt(lua_State*);
}

#endif
