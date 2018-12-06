project('hdr2sdr', 'c', 'cpp',
	default_options : ['c_std=c11', 'cpp_std=c++11'])

src = files(
	'hdr_decoder.cpp',
	'image.cpp',
	'image_decoder.cpp',
	'image_encoder.cpp',
	'image_merge.cpp',
	'ldr_decoder.cpp',
	'params.cpp',
	'pfm_decoder.cpp',
	'openexr_decoder.cpp',
	'stb_image.c',
	'tonemapper.cpp')

include_dir = include_directories('third-party/install/include', 'include')
include_public_dir = include_directories('include')
jpeg_dep = dependency('libjpeg', version : '>= 2.0.0')

libhdr2sdr = shared_library('hdr2sdr', src, include_directories : include_dir, dependencies : [jpeg_dep])
libhdr2sdr_dep = declare_dependency(include_directories : include_public_dir, link_with : libhdr2sdr)

executable('image_decoder_unittest', 'image_decoder_unittest.cpp', include_directories : include_dir, link_with : libhdr2sdr)
executable('image_encoder_unittest', 'image_encoder_unittest.cpp', include_directories : include_dir, link_with : libhdr2sdr)
executable('image_merge_unittest', 'image_merge_unittest.cpp', include_directories : include_dir, link_with : libhdr2sdr)
executable('tonemapper_unittest', 'tonemapper_unittest.cpp', include_directories : include_dir, link_with : libhdr2sdr)
