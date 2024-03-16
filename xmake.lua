add_rules("mode.debug", "mode.release")

includes("deps")

add_requires("reflect")

target("DynamicType")
    set_kind("binary")
    
    set_languages("cxx23")

    add_headerfiles("include/**.hpp", { public = true })
    add_includedirs("include", { public = true })

    add_files("src/*.cpp")

    add_extrafiles(".clang-format")

    add_packages("reflect", { public = true })

includes("tests")