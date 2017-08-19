sources = [
    'gtkimageview.c',
    'gtkimageview.h',
    ]

libgtk = dependency('gtk+-3.0', version: '>= 3.7.10')

cc = meson.get_compiler ('c')

libm = cc.find_library ('libm')

libgtkimageview = static_library ('gtkimageview',
                                   sources,
                                   dependencies : [libgtk, libm])
libgtkimageviewdep = declare_dependency (
    link_with: libgtkimageview,
    sources: sources)
