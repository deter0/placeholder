project('placeholder', 'c')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : true)

add_global_arguments('-Wno-unused-function', language : ['c'])
add_global_arguments('-Wno-unused-variable', language : ['c'])

inc_dirs = include_directories(
	'libs',
	'include',
	'placeholder'
)

dependencies = [
	dependency('allegro-5'),
	dependency('allegro_font-5'),
	dependency('allegro_ttf-5'),
	dependency('allegro_image-5'),
	dependency('allegro_primitives-5'),
	m_dep
]

sources = [
	'placeholder/main.c',
	'placeholder/world.c',
	'placeholder/fonts.c',
	'placeholder/instances/instances.c',
	'placeholder/instances/services/input_service.c',
	'placeholder/hashes.c',
	'placeholder/util.c',
	'placeholder/plog.c'
]

executable('placeholder', sources : sources, dependencies : dependencies, include_directories : inc_dirs)
