add_requires("gtest")

rule("test")
    on_load(function (target)
        target:set("kind", "binary")
        target:set("languages", "cxx23")

        target:set("exceptions", "cxx")

        target:set("default", "false")

        target:set("group", "tests")

        target:add("deps", "DynamicType")
        target:add("packages", "gtest")
    end)

target("TSingle")
    add_rules("test")

    add_files("TSingle.cpp")
    add_tests("default")

target("TOption")
    add_rules("test")

    add_files("TOption.cpp")

    add_tests("first", { runargs = "--gtest_filter=TOption.First" })
    add_tests("second", { runargs = "--gtest_filter=TOption.Second" })
    add_tests("third", { runargs = "--gtest_filter=TOption.Third" })
    add_tests("exception", { runargs = "--gtest_filter=TOption.Exception" })


target("TEither")
    add_rules("test")

    add_files("TEither.cpp")

    add_tests("left", { runargs = "--gtest_filter=TEither.Left" })
    add_tests("right", { runargs = "--gtest_filter=TEither.Right" })

target("Mixed")
    add_rules("test")

    add_files("Mixed.cpp")

    add_tests("everything", { runargs = "--gtest_filter=TMixed.Everything" })
    add_tests("noOptions", { runargs = "--gtest_filter=TMixed.NoOptions" })
    add_tests("leftSelectWithOptions", { runargs = "--gtest_filter=TMixed.LeftSelectWithOptions" })
    add_tests("leftSelectNoOption", { runargs = "--gtest_filter=TMixed.LeftSelectNoOption" })

target("Nested")
    add_rules("test")

    add_files("Nested.cpp")
    add_tests("default")