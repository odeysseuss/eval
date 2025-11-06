add_rules("mode.debug", "mode.release")

target("eval")
set_kind("binary")
set_languages("c99")
set_warnings("all")
set_optimize("faster")
add_includedirs("include", "utils")
add_files(
    "src/main.c",
    "src/lexer.c",
    "utils/str.c"
)
