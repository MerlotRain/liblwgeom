glib_dep = dependency('glib-2.0')

zlog_enabled = get_option('zlog')
zlog_include_dir = get_option('zlog_include_dir')
zlog_lib_dir = get_option('zlog_lib_dir')

if zlog_enabled
    zlog_dep = declare_dependency(
        include_directories: include_directories(zlog_include_dir),
        link_args: ['-L' + zlog_lib_dir, '-lzlog'],
    )
else
    zlog_dep = dependency('zlog', required: false)
endif

util_src = files(
    'bitset.c',
    'lwutil.c',
    'sda.c',
)

util_inc = include_directories('.')

lw_util_lib = static_library(
    'lw_util',
    sources: util_src,
    include_directories: util_inc,
    dependencies: [glib_dep, zlog_dep],
)