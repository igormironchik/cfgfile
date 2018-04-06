from conans import ConanFile


class ArgsConan(ConanFile):
    name = "cfgfile"
    version = "0.2.8"
    url = "https://github.com/igormironchik/cfgfile.git"
    license = "MIT"
    description = "Header-only library for reading/saving configuration files with schema defined in sources."
    exports = "cfgfile/*", "COPYING", "generator/*", "3rdparty/Args/Args/*.hpp"

    def package(self):
        self.copy("COPYING", src=".", dst=".")
        self.copy("*", src="cfgfile", dst="cfgfile")
        self.copy("*", src="generator", dst="generator")
        self.copy("*.hpp", src="3rdparty/Args/Args", dst="3rdparty/Args/Args")
        
    def package_info(self):
        self.cpp_info.includedirs = ["."]
