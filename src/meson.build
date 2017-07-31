xkcd_sources = [
    'xkcds-application.c',
    'xkcds-window.c',
    'main.c',
]

xkcd_deps = [
    dependency ('glib-2.0'),
    dependency ('gtk+-3.0'),
]

resources = gnome.compile_resources ('xkcd-resources',
                                     'resources/xkcds-gresources.xml',
                                     source_dir: 'resources')
xkcd_sources += resources

executable ('xkcd', xkcd_sources,
    dependencies: xkcd_deps,
    install: true
)
