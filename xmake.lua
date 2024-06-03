add_rules("mode.debug", "mode.release")

add_repositories("deps deps")
add_requires("reflect")

target("DynamicType")
    set_kind("headeronly")
    set_warnings("all", "error")
    
    set_languages("cxx23")

    add_headerfiles("include/**.hpp", { public = true })
    add_includedirs("include", { public = true })

    add_extrafiles(".clang-format")

    add_cxxflags("/Zc:preprocessor", { tools = { "clang_cl", "cl" }, public = true })

    add_packages("reflect", { public = true })

includes("tests")