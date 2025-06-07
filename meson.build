project('itch_parser', 'cpp',
  version : '0.1.0',
  default_options : ['warning_level=3',
                     'cpp_std=c++23'])

# Include directories
inc_dir = include_directories('src')

# Find all C++ source files in src directory
src_files = files(
  # Add your source files here, for example:
  'src/main.cpp',
  # 'src/parser.cpp',
  # 'src/utils.cpp'
)

# You can also use a glob pattern to automatically find all cpp files
cpp_files = run_command('find', 'src', '-name', '*.cpp', '-type', 'f', check: false)
if cpp_files.returncode() == 0
  src_files = files(cpp_files.stdout().strip().split('\n'))
endif

# Define the executable
executable('itch_parser',
  sources : src_files,
  include_directories : inc_dir,
  install : true)

# Optional: Add dependencies if needed
# For example, if you need json parsing:
# json_dep = dependency('nlohmann_json', required : false)
# if json_dep.found()
#   executable('itch_parser',
#     sources : src_files,
#     include_directories : inc_dir,
#     dependencies : [json_dep],
#     install : true)
# endif

# Optional: Add a library target if you want to build a library too
# itch_parser_lib = library('itch_parser',
#   sources : src_files,
#   include_directories : inc_dir,
#   install : true)

# Optional: Add tests
# if get_option('tests')
#   test_files = files(
#     'tests/test_main.cpp'
#   )
#
#   test_exe = executable('test_itch_parser',
#     sources : test_files + src_files,
#     include_directories : inc_dir)
#
#   test('itch_parser_tests', test_exe)
# endif
