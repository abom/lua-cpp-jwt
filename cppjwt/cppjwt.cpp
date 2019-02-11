#include <cppjwt.h>

template<typename... AlgArgs>
jwt::algorithm::hmacsha get_hs_alg_by_name(const std::string& name, AlgArgs... args) {
    if (name == "HS256")
        return jwt::algorithm::hs256(args...);
    if (name == "HS384")
        return jwt::algorithm::hs384(args...);
    if (name == "HS512")
        return jwt::algorithm::hs512(args...);

    throw unsupported_algorithm(name.c_str());
}

template<typename... AlgArgs>
jwt::algorithm::rsa get_rs_alg_by_name(const std::string& name, AlgArgs... args) {
    if (name == "RS256")
        return jwt::algorithm::rs256(args...);
    if (name == "RS384")
        return jwt::algorithm::rs384(args...);
    if (name == "RS512")
        return jwt::algorithm::rs512(args...);

    throw unsupported_algorithm(name.c_str());
}

template<typename... AlgArgs>
jwt::algorithm::ecdsa get_es_alg_by_name(const std::string& name, AlgArgs... args) {
    if (name == "ES256")
        return jwt::algorithm::es256(args...);
    if (name == "ES384")
        return jwt::algorithm::es384(args...);
    if (name == "ES512")
        return jwt::algorithm::es512(args...);

    throw unsupported_algorithm(name.c_str());
}

template<typename... AlgArgs>
jwt::algorithm::pss get_ps_alg_by_name(const std::string& name, AlgArgs... args) {
    if (name == "PS256")
        return jwt::algorithm::ps256(args...);
    if (name == "PS384")
        return jwt::algorithm::ps384(args...);
    if (name == "PS512")
        return jwt::algorithm::ps512(args...);

    throw unsupported_algorithm(name.c_str());
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
        std::string alg_type = alg_name.substr(0, 2);

        // FIXME: args other than key can be passed too
        if (alg_type == "ES") {
            jwt::verify().allow_algorithm(get_es_alg_by_name(alg_name, key)).verify(decoded_token);
        } else if (alg_type == "HS") {
            jwt::verify().allow_algorithm(get_hs_alg_by_name(alg_name, key)).verify(decoded_token);
        } else if (alg_type == "PS") {
            jwt::verify().allow_algorithm(get_ps_alg_by_name(alg_name, key)).verify(decoded_token);
        } else if (alg_type == "RS") {
            jwt::verify().allow_algorithm(get_rs_alg_by_name(alg_name, key)).verify(decoded_token);
        } else {
            throw unsupported_algorithm(alg_name.c_str());
        }

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
