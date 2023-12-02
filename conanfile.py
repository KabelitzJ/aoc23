import os
from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, CMake, cmake_layout
from conan.tools.build import check_max_cppstd, check_min_cppstd

class libsbx_recipe(ConanFile):
  name = "aoc23"
  version = "0.1.0"
  
  # Metadata
  license = "MIT"
  author = "Jonas Kabelitz <jonas-kabelitz@gmx.de>"

  # Binary configuration
  settings = (
    "os", 
    "compiler", 
    "build_type", 
    "arch"
  )
  options = {
    "fPIC": [True, False],
  }
  default_options = {
    "fPIC": True,
  }

  def config_options(self):
    if self.settings.os == "Windows":
      self.options.fPIC = False

  def layout(self):
    cmake_layout(self)

    # Define custom layout for build artifacts

    is_compiler_multi_config = self.settings.compiler == "msvc"

    self.folders.build = "build"

    if not is_compiler_multi_config:
      self.folders.build = os.path.join(self.folders.build, str(self.settings.build_type).lower())

    self.folders.generators = os.path.join(self.folders.build, "dependencies")

  def requirements(self):
    self.requires("fmt/10.0.0")
    self.requires("range-v3/0.12.0")

  def generate(self):
    deps = CMakeDeps(self)
    toolchain = CMakeToolchain(self)

    deps.generate()
    toolchain.generate()

  def build(self):
    cmake = CMake(self)

    variables = {
      "SBX_BUILD_DEMO": "On" if self.options.build_demo else "Off",
      "SBX_BUILD_SHARED": "On" if self.options.shared else "Off",
      "SBX_BUILD_TESTS": "On" if self.options.build_tests else "Off"
    }

    cmake.configure(variables)
    cmake.build()

  def package(self):
    cmake = CMake(self)
    cmake.install()

  def package_info(self):
    self.cpp_info.components["core"].libs = ["core"]
    self.cpp_info.components["core"].includedirs = ["include"]
