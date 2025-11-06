add_rules("mode.debug", "mode.release")

target("eval")
set_kind("binary")
set_languages("c99", "c++17")
set_warnings("all", "error")
set_optimize("faster")
add_includedirs("include", "utils")
add_files("src/*.c")
