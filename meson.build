project('qenqey', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=2',
                     'cpp_std=c++17'])

qt6 = import('qt6')
qt6_dep = dependency('qt6', modules: ['Core', 'Widgets'])

# MOC processing
qt6_processed = qt6.compile_moc(
  sources : 'src/main.cpp',
  extra_args : ['-DMOC_PREDEFS_H']
)

# Application executable
executable('qenqey',
  sources : ['src/main.cpp', qt6_processed],
  dependencies : [qt6_dep]
)