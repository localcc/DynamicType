add_requires("gtest")

target("TSingle")
    set_kind("binary")
    set_languages("cxx23")

    set_default("false")
    add_files("TSingle.cpp")

    set_group("tests")

    add_tests("default")

    add_deps("DynamicType")
    add_packages("gtest")

target("TOption")
    set_kind("binary")
    set_languages("cxx23")

    set_default("false")
    add_files("TOption.cpp")

    set_group("tests")

    add_tests("first", { runargs = "--gtest_filter=TOption.First" })
    add_tests("second", { runargs = "--gtest_filter=TOption.Second" })
    add_tests("third", { runargs = "--gtest_filter=TOption.Third" })
    add_tests("exception", { runargs = "--gtest_filter=TOption.Exception" })

    add_deps("DynamicType")
    add_packages("gtest")

target("TEither")
    set_kind("binary")
    set_languages("cxx23")

    set_exceptions("cxx")

    set_default("false")
    add_files("TEither.cpp")

    set_group("tests")

    add_tests("left", { runargs = "--gtest_filter=TEither.Left" })
    add_tests("right", { runargs = "--gtest_filter=TEither.Right" })

    add_deps("DynamicType")
    add_packages("gtest")

target("Mixed")
    set_kind("binary")
    set_languages("cxx23")

    set_exceptions("cxx")

    set_default("false")
    add_files("Mixed.cpp")

    set_group("tests")

    add_tests("everything", { runargs = "--gtest_filter=TMixed.Everything" })
    add_tests("noOptions", { runargs = "--gtest_filter=TMixed.NoOptions" })
    add_tests("leftSelectWithOptions", { runargs = "--gtest_filter=TMixed.LeftSelectWithOptions" })
    add_tests("leftSelectNoOption", { runargs = "--gtest_filter=TMixed.LeftSelectNoOption" })

    add_deps("DynamicType")
    add_packages("gtest")