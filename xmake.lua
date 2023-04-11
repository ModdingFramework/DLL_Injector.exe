add_rules("mode.debug")
set_toolchains("msvc")
set_languages("c++20")

add_repositories("MrowrLib https://github.com/MrowrLib/Packages.git")
add_repositories("ModdingFramework https://github.com/ModdingFramework/Packages.git")

add_requires("DLL_Injection", "UserInterface", "StringFormatting", "spdlog", "simpleini", "nana")

target("DLL_Injector")
    set_kind("binary")
    add_ldflags("/subsystem:windows")
    set_policy("check.auto_ignore_flags", false)
    add_ldflags("/entry:WinMainCRTStartup")
    add_files("DLL_Injector_UI.cpp", "app.rc")
    add_packages("DLL_Injection", "UserInterface", "StringFormatting", "spdlog", "simpleini", "nana")
