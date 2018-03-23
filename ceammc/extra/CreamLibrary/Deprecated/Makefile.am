AUTOMAKE_OPTIONS = foreign
noinst_LTLIBRARIES = libDeprecated.la

libDeprecated_la_CXXFLAGS = @PD_CPPFLAGS@ -O3 -fPIC
libDeprecated_la_LDFLAGS = -module -avoid-version -shared -shrext .@EXTENSION@  @PD_LDFLAGS@ -fPIC
libDeprecated_la_LIBADD = 

libDeprecated_la_SOURCES = c.pak.cpp \
c.prepend.cpp \
c.loadmess.cpp \
c.patcherargs.cpp \
c.patcherinfos.cpp \
c.patchermess.cpp \
AudioFFT.cpp \
AudioFFT.h \
c.convolve_tilde.cpp \
c.freeverb_tilde.cpp \
FFTConvolver.cpp \
FFTConvolver.h \
Utilities.cpp \
Utilities.h
