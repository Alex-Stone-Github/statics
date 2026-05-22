project('statics', 'cpp', version: '0.1',
  default_options: ['cpp_std=c++23', 'warning_level=3'])

vulkan = dependency('vulkan', required: true)
sdl2 = dependency('sdl2', fallback: 'sdl2', required: true)
gtest = dependency('gtest', required: true)

# Helper Library
wrap = library('wrap', 
  'wrap_instance.cpp', 'wrap_device.cpp', 'wrap_internal.cpp', 'wrap_draw.cpp',
  install: true,
  dependencies: [vulkan])

wrap_sdl = library('wrap_sdl', 
  'wrap_sdlctx.cpp',
  install: true,
  dependencies: [vulkan, sdl2])

# Helper Library Headers
install_headers('wrap.h')
install_headers('wrap_sdlctx.h')

# Helper Library Tests
wrap_tests = executable('wrap_tests', 'tests.cpp',
  install: false,
  link_with: [wrap, wrap_sdl],
  dependencies: [vulkan, sdl2, gtest])
test('Wrap Library Tests', wrap_tests)

# Executable App
statics = executable('statics', 'main.cpp', 
  install: true, 
  link_with: [wrap, wrap_sdl],
  dependencies: [vulkan, sdl2])

