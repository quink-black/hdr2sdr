project('hdr2sdr', 'c', 'cpp',
	default_options : ['c_std=c11', 'cpp_std=c++11'])

src = files('image.cpp',
	'image_decoder.cpp',
	'image_encoder.cpp',
	'pfm_decoder.cpp',
	'openexr_decoder.cpp',
	'tonemapper.cpp')

include_dir = include_directories('third-party/install/include')
jpeg_dep = dependency('libjpeg', version : '>= 2.0.0')

lib = shared_library('hdr2sdr', src, dependencies : [jpeg_dep])

executable('image_decoder_unittest', 'image_decoder_unittest.cpp', link_with : lib)
executable('image_encoder_unittest', 'image_encoder_unittest.cpp', link_with : lib)
executable('tonemapper_unittest', 'tonemapper_unittest.cpp', link_with : lib)
