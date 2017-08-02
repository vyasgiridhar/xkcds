xkcd_sources = [
    'xkcds-application.c',
    'xkcds-window.c',
    'xkcd.c',
    'xkcd-api.c',
    'main.c',
]

xkcd_deps = [
    dependency ('gtk+-3.0'),
    dependency ('libsoup-2.4'),
    dependency('json-glib-1.0'),
]

resources = gnome.compile_resources ('xkcd-resources',
                                     'resources/xkcds-gresources.xml',
                                     source_dir: 'resources')
xkcd_sources += resources

executable ('xkcd', xkcd_sources,
    dependencies: xkcd_deps,
    install: true
)
