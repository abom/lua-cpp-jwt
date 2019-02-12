#include <cppjwt.h>

template<typename... AlgArgs>
void allow_algorithm_by_name(Verifier& verifier, const std::string& name, AlgArgs... args) {
    if (name == "HS256") {
        verifier.allow_algorithm(jwt::algorithm::hs256(args...));
    } else if (name == "HS384") {
        verifier.allow_algorithm(jwt::algorithm::hs384(args...));
    } else if (name == "HS512") {
        verifier.allow_algorithm(jwt::algorithm::hs512(args...));
    } else if (name == "RS256") {
        verifier.allow_algorithm(jwt::algorithm::rs256(args...));
    } else if (name == "RS384") {
        verifier.allow_algorithm(jwt::algorithm::rs384(args...));
    } else if (name == "RS512") {
        verifier.allow_algorithm(jwt::algorithm::rs512(args...));
    } else if (name == "ES256") {
        verifier.allow_algorithm(jwt::algorithm::es256(args...));
    } else if (name == "ES384") {
        verifier.allow_algorithm(jwt::algorithm::es384(args...));
    } else if (name == "ES512") {
        verifier.allow_algorithm(jwt::algorithm::es512(args...));
    } else if (name == "PS256") {
        verifier.allow_algorithm(jwt::algorithm::ps256(args...));
    } else if (name == "PS384") {
        verifier.allow_algorithm(jwt::algorithm::ps384(args...));
    } else if (name == "PS512") {
        verifier.allow_algorithm(jwt::algorithm::ps512(args...));
    } else {
        throw unsupported_algorithm(name.c_str());
    }
}

char* decode(const char* token) {
    try {
        auto decoded_token = jwt::decode(token);
        picojson::object token_obj;
        picojson::value header, payload, signature;

        picojson::parse(header, decoded_token.get_header());
        picojson::parse(payload, decoded_token.get_payload());
        signature = picojson::value(decoded_token.get_signature());

        token_obj.insert(std::pair<std::string, picojson::value>("header", header));
        token_obj.insert(std::pair<std::string, picojson::value>("payload", payload));
        token_obj.insert(std::pair<std::string, picojson::value>("signature", signature));

        std::string json = picojson::value(token_obj).serialize();
        return strdup(json.c_str());
    } catch(const std::invalid_argument& ex) {
        return NULL;
    } catch(const std::runtime_error& ex) {
        return NULL;
    }
}

int verify(const char* token, const char* key) {

    try {
        auto decoded_token = jwt::decode(token);
        std::string alg_name = decoded_token.get_algorithm();

        Verifier verifier = jwt::verify();
        allow_algorithm_by_name(verifier, alg_name, key);
        verifier.verify(decoded_token);

        return 0;
    } catch(const jwt::token_verification_exception&) {
        return -1;
    } catch (const unsupported_algorithm& ex) {
        std::cerr << ex.what();
        return -2;
    } catch(const std::runtime_error& ex) {
        std::cerr << ex.what();
        return -3;
    }
}

static int lua_jwt_decode(lua_State* L) {
    const char* token = luaL_checkstring(L, 1);
    char* ret = decode(token);

    if (ret == NULL) {
        lua_pushstring(L, "Invalid json");
        lua_error(L);
    }

    lua_pushstring(L, ret);
    free(ret);
    return 1;
}

static int lua_jwt_verify(lua_State* L) {
    const char* token = luaL_checkstring(L, 1);
    const char* key = luaL_checkstring(L, 2);
    int ret = verify(token, key);

    if (ret == -2) {
        lua_pushstring(L, "Unsupported algorithm");
        lua_error(L);
    } else if (ret == -3) {
        lua_pushfstring(L, "Runtime error, check if token and key are valid");
        lua_error(L);
    }

    lua_pushboolean(L, ret == 0);
    return 1;
}

int luaopen_libcppjwt(lua_State* L) {
    luaL_newlib(L, cppjwt);
    return 1;
}
