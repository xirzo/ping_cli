project(
  'ping_cli',
  'c',
  meson_version : '>= 1.3.0',
  version : '0.0.1',
  default_options : [
    'warning_level=3',
    'c_std=c99',
  ],
)

c = meson.get_compiler('c')

inc = include_directories('src/')

sources = files(
  'src/main.c',
  'src/echo_packet.c',
  'src/ping.c',
)

build_args = []

if c.get_id() in ['gcc', 'clang']
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
    language: ['c'],
  )
  add_project_link_arguments(
    '-Wl,--gc-sections', '-Wl,--as-needed',
    language: ['c'],
  )
endif

exe = executable(
  'ping',
  sources,
  include_directories : inc,
  install : true,
  c_args : build_args,
)
