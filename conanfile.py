import os
from conans import ConanFile, CMake, tools
from conans.tools import download, unzip

class VclConan(ConanFile):
    name = "vcl"
    version = "2ea4dec"
    generators = "cmake"
    settings = "os","compiler","build_type","arch"
    options = { 
            "vectorization": ["AVX", "AVX2", "SSE4_2" ], 
            "fPIC": [True, False]
            }
    default_options = \
            "vectorization=AVX", \
            "fPIC=False"

    requires = (("Eigen3/3.3.3@bschindler/testing"))

    exports = ["FindVcl.cmake"]
    url="https://github.com/bfierz/vcl.git"
    license="MIT"
    description="Visual Computing Library (VCL)"
    exports_sources = "src/*"

    ZIP_FOLDER_NAME = "%s" % version

    def source(self):
        tools.replace_in_file("src/CMakeLists.txt", "PROJECT(VisualComputingLibrary)", '''PROJECT(VisualComputingLibrary)
                include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
                conan_basic_setup()
                set(VCL_EIGEN_DIR ${CONAN_EIGEN3_ROOT})''')

    def config_options(self):
        if self.settings.compiler == "Visual Studio":
            self.options.remove("fPIC")

    def build(self):
        vectorization_key = "VCL_VECTORIZE_" + str(self.options.vectorization) + ":BOOL"

        defs={"VCL_BUILD_BENCHMARKS:BOOL":"off",
            "VCL_BUILD_TESTS:BOOL":"on",
            "VCL_BUILD_TOOLS:BOOL":"off",
            "VCL_BUILD_EXAMPLES:BOOL" : "off",
            vectorization_key : "on"
        }

        if self.options.fPIC:
            defs["CMAKE_POSITION_INDEPENDENT_CODE:BOOL"] = "on"

        cmake = CMake(self.settings)
        cmake.configure(self, source_dir=self.conanfile_directory + "/src/", build_dir="./", defs=defs)
        cmake.build(self, target="vcl_geometry")
        cmake.build(self, target="vcl_math")

    def package(self):
        self.copy("*.a", dst="lib", src="lib")
        self.copy("*.h", dst="include", src="src/libs")
        self.copy("config.h", dst="include/vcl.core/vcl/config", src="libs/vcl.core/vcl/config")

    def package_info(self):
        self.cpp_info.includedirs = ['include/vcl.core', 'include/vcl.math', 'include/vcl.geometry']
        self.cpp_info.libs = ['libvcl_core.a', 'libvcl_math.a', 'libvcl_geometry.a']
        self.cpp_info.libdirs = [ "lib" ]
