
import pkgboot

class Http(pkgboot.Package):
    defines = {}
    includes = []
    libs = [
        'coro',
        pkgboot.Lib('ws2_32', 'win32'),
        pkgboot.Lib('advapi32', 'win32'),
        pkgboot.Lib('user32', 'win32'),
        pkgboot.Lib('gdi32', 'win32'),
        'libeay32',
        'ssleay32',
    ]
    major_version = '0'
    minor_version = '0'
    patch = '0'

Http()
