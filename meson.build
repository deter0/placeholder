project('placeholder', 'c')

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : true)

inc_dirs = include_directories('libs', 'include')

dependencies = [
	dependency('allegro-5'),
	dependency('allegro_font-5'),
	dependency('allegro_ttf-5'),
	dependency('allegro_image-5'),
	dependency('allegro_primitives-5'),
	m_dep
]

sources = [
	'src/main.c',
	'src/characters.c',
	'src/all_characters.c',
	'src/world.c',
	'src/fonts.c',
	'src/instances/instances.c',
	'src/hashes.c',
]

executable('placeholder', sources : sources, dependencies : dependencies, include_directories : inc_dirs)
