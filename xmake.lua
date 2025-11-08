add_rules("mode.debug", "mode.release")

target("eval")
set_kind("binary")
set_languages("c99")
set_warnings("all")
if is_mode("release") then
    set_optimize("faster")
end
add_includedirs("include", "utils")
add_files(
    "src/main.c",
    "src/lexer.c",
    "src/parser.c",
    "utils/str.c"
)
add_syslinks("m")
