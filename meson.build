# compiler: gcc

project('interpreter', 'c',
        version : '1.0.0',
        default_options : ['warning_level=3', 'c_std=c23'])

subdir('src')
deps = include_directories('deps')
extra_args = ['-fshort-enums']
interpreter = executable('interpreter', sources, install: true, include_directories: deps, c_args: extra_args)

test('test', interpreter)
