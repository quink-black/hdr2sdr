project('hdr2sdr', 'c', 'cpp',
	default_options : ['c_std=c11', 'cpp_std=c++11'])

src = files('image_decoder.cpp',
	'image_encoder.cpp',
	'tonemapper.cpp')

lib = shared_library('hdr2sdr', src)

executable('image_decoder_unittest', 'image_decoder_unittest.cpp', link_with : lib)
executable('tonemapper_unittest', 'tonemapper_unittest.cpp', link_with : lib)
