from conans import ConanFile, CMake


class ArgsConan(ConanFile):
    name = "cfgfile"
    version = "0.2.8.2.1"
    url = "https://github.com/igormironchik/cfgfile.git"
    license = "MIT"
    description = "Header-only library for reading/saving configuration files with schema defined in sources."
    exports = "cfgfile/*", "COPYING", "generator/*", "3rdparty/Args/Args/*.hpp"
    generators = "cmake"
    
    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="generator")
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("COPYING", src=".", dst=".")
        self.copy("*", src="cfgfile", dst="cfgfile")
        self.copy("*", src="generator", dst="generator")
        self.copy("*.hpp", src="3rdparty/Args/Args", dst="3rdparty/Args/Args")
        
    def package_info(self):
        self.cpp_info.includedirs = ["."]
