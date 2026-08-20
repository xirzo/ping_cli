project(
  'ping_cli',
  'cpp',
  meson_version : '>= 1.3.0',
  version : '0.0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++26',
  ],
)

cpp = meson.get_compiler('cpp')

inc = include_directories('src/')

sources = files(
  'src/main.cpp',
)

build_args = []

if cpp.get_id() in ['gcc', 'clang']
  build_args += [
    '-Wpedantic',
    '-Wconversion',
    '-Wsign-conversion',
    '-Wshadow',
  ]
endif

if get_option('buildtype') == 'release'
  add_project_arguments(
    '-ffunction-sections', '-fdata-sections',
    language: ['c', 'cpp'],
  )
  add_project_link_arguments(
    '-Wl,--gc-sections', '-Wl,--as-needed',
    language: ['c', 'cpp'],
  )
endif

exe = executable(
  'ping',
  sources,
  include_directories : inc,
  install : true,
  cpp_args : build_args,
)

test_sources = files(
  'tests/test_foo.cpp',
)

gtest_dep = dependency('gtest', main : true, required : true, fallback : ['gtest', 'gtest_main_dep'])

if test_sources.length() > 0 and get_option('buildtype') != 'release'
  test_exe = executable(
    'ping_tests',
    test_sources,
    include_directories : inc,
    dependencies : gtest_dep,
    cpp_args : build_args,
  )
  test('unit_tests', test_exe, protocol : 'gtest')
endif
