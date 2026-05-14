project('statics', 'cpp', version: '0.1',
  default_options: ['cpp_std=c++23', 'warning_level=3'])

vulkan = dependency('vulkan', required: true)
vulkan = dependency('sdl2', required: true)

deps = [vulkan]

# Helper Library
wrap = library('wrap', 'wrap.cpp', 
  install: true,
  dependencies: deps)

# Executable App
statics = executable('statics', 'main.cpp', 
  install: true, 
  link_with: wrap,
  dependencies: deps)
