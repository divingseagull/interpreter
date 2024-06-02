# compiler: gcc

project('interpreter', 'c',
        version : '1.0.0',
        default_options : ['warning_level=3', 'c_std=c11'])
subdir('src')
deps = include_directories('deps')
interpreter = executable('interpreter', sources, install: true, include_directories: deps)

test('test', interpreter)
