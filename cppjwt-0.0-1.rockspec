package = "cppjwt"
   version = "0.0-1"
   source = {
      url = "https://github.com/abom/lua-cpp-jwt"
   }
   build = {
      type = "make",
      build_variables = {
         CFLAGS="$(CFLAGS)",
         LIBFLAG="$(LIBFLAG)",
         LUA_LIBDIR="$(LUA_LIBDIR)",
         LUA_BINDIR="$(LUA_BINDIR)",
         LUA_INCDIR="$(LUA_INCDIR)",
         LUA="$(LUA)",
      },
      install_variables = {
         INST_PREFIX="$(PREFIX)",
         INST_BINDIR="$(BINDIR)",
         INST_LIBDIR="$(LIBDIR)",
         INST_LUADIR="$(LUADIR)",
         INST_CONFDIR="$(CONFDIR)",
      },
   }
