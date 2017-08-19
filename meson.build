project ('xkcds', 'c',
    version: '0.1.0',
    meson_version: '>= 0.36.0',
)

gnome = import ('gnome')

config_h = configuration_data()
config_h.set_quoted ('GETTEXT_PACKAGE', 'xkcds')
config_h.set_quoted ('LOCALE_DIR', join_paths(get_option ('prefix'), get_option ('localedir')))

configure_file(
    output: 'config.h',
    configuration: config_h,
)

add_global_arguments ([
    '-DHAVE_CONFIG_H',
    '-I' + meson.build_root(),
    ], language: 'c')

subdir ('libgtkimageview')
subdir ('src')
subdir ('po')
