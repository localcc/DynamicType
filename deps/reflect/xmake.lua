package("reflect")
    add_urls("https://github.com/boost-ext/reflect.git")
    add_urls("git@github.com:boost-ext/reflect.git")

    add_versions("v1.1.0", "5c637d2acbf0496f19437f20842253d7a47b42ef")

    on_install(function (package)
        local xmake_lua = ([[
            add_rules("mode.debug", "mode.release")

            target("reflect")
                set_kind("headeronly")
                set_languages("cxx23")

                add_includedirs(".", { public = true })
                add_headerfiles("reflect")
        ]])
        io.writefile("xmake.lua", xmake_lua)

        import("package.tools.xmake").install(package)
    end)