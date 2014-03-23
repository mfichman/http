
import platform
import os
import sys
import shutil
import subprocess
import shlex
import glob

try:
    from SCons.Script import *
except ImportError:
    pass

def run_test(target, source, env):
# Runs a single unit test and checks that the return code is 0
    try:
        subprocess.check_call(source[0].abspath)
    except subprocess.CalledProcessError, e:
        return 1

def build_pch(target, source, env):
    try:
        args = (
            str(env['CXX']),
            str(env['CXXFLAGS']),
            ' '.join('-I%s' % path for path in env['CPPPATH']), 
            '-x',
            'c++-header',
            str(source[0]),
            '-o',
            str(target[0]),
        )
        print(' '.join(args))
        subprocess.check_call(shlex.split(' '.join(args)))
    except subprocess.CalledProcessError, e:
        return 1

class Lib:
    """
    Defines a library that the build depends upon.
    """
    def __init__(self, name, platforms):
        self.name = name
        self.platforms = platforms

    def __str__(self):
        return self.name

    def __repr__(self):
        return self.name

class Package:
    """
    Defines a workspace for a package.  Automatically sets up all the usual SCons 
    stuff, including a precompiled header file.
    """
    defines = {}
    includes = []
    libs = []
    frameworks = []
    path = []
    lib_path = []
    major_version = '0'
    minor_version = '0'
    patch = '0'
    kind = 'lib'
    frameworks = []
    assets = []

    def __init__(self):
        # Initializes a package, and sets up an SCons build environment given
        # the instructions found in the subclass definition.
        self._setup_vars()
        self._setup_env()
        self._setup_assets()
        if self.env['PLATFORM'] == 'win32':
            self._setup_win()
        else:
            self._setup_unix()
        self._setup_tests()
        self.build() # Run custom code for building the package

    def _lib_is_valid_for_platform(self, lib):
        if type(lib) == str:
            return True
        elif type(lib.platforms) == list:
            return self.env['PLATFORM'] in lib.platforms
        elif type(lib.platforms) == str:
            return self.env['PLATFORM'] == lib.platforms

    def _setup_assets(self):
        if len(self.assets) <= 0:
            return

        fd = open('include/%s/Assets.hpp' % self.name, 'w')
        fd.write('#pragma once\n')
        fd.write('namespace %s {\n' % self.name)
        fd.write('struct Asset {\n')
        fd.write('    char const* name;\n')
        fd.write('    char const* data;\n')
        fd.write('    size_t len;\n')
        fd.write('};\n')
        fd.write('extern Asset const assets[];\n')
        fd.write('}\n')
        fd.close()

        fd = open('src/Assets.cpp', 'w')
        fd.write('#include "%s/Common.hpp"\n' % self.name)
        fd.write('#include "%s/Assets.hpp"\n' % self.name)
        fd.write('namespace %s {\n' % self.name)
        fd.write('extern Asset const assets[] = {\n')
        for pattern in self.assets:
            for fn in glob.glob(pattern):
                fn = fn.replace('\\', '/') # Windows!!
                fd.write('{"%s",' % fn) 
                r = open(fn, 'rb') # Windows!!! (binary mode required)
                data = r.read()
                length = len(data)
                data = ''.join(['\\x%02x' % ord(ch) for ch in data])
                fd.write('"%s",%d},\n' % (data, length))
        fd.write('{0, 0, 0},')
        fd.write('};\n')
        fd.write('}\n')
        fd.close()
                  
    
    def _setup_vars(self):
        # Set up the basic configuration options
        (system, _, release, version, machine, proc) = platform.uname()
        self.name = self.__class__.__name__.lower()
        self.pch = '%s/Common.hpp' % self.name
        self.build_mode = ARGUMENTS.get('mode', 'debug')
        self.version = '.'.join((self.major_version, self.minor_version, self.patch))
        self.branch = os.popen('git rev-parse --abbrev-ref HEAD').read().strip()
        self.revision = os.popen('git rev-parse HEAD').read().strip()
        self.defines.update({
            'VERSION': self.version,
            'REVISION': self.revision,
            'BRANCH': self.branch,
        })
        self.includes.extend([
            'include',
            'src', 
        ])
        self.src = []

    def _setup_env(self):
        # Create the SCons build environment, and fill it with default parameters.
        self.env = Environment(CPPPATH=['build/src'])
        self.env.Append(ENV=os.environ)
        self.env.VariantDir('build/src', 'src', duplicate=0)
        self.env.VariantDir('build/test', 'test', duplicate=0)

    def _setup_win(self):
        # Set up Windows-specific options
        if self.build_mode == 'debug':
            pass 
        elif self.build_mode == 'release':
            self.env.Append(CXXFLAGS='/O2')
        else:
            assert not "Unknown build type"
        self.env.Append(CXXFLAGS='/W4 /WX /wd4100 /MT /EHsc /Zi /Gm /FS')
        self.env.Append(CXXFLAGS='/Fpbuild/Common.pch')
        self.env.Append(CXXFLAGS='/Yu%s' % self.pch)
        self.env.Append(LINKFLAGS='/DEBUG')
        self.src += self.env.Glob('build/src/**.asm')
        # Add MASM assembly files

        self.includes.extend([
            'C:\\WinBrew\\include',
        ])
        self.lib_path.extend([
            'C:\\WinBrew\\lib',
        ])
        self.path.extend([
            os.environ['PATH'],
            'C:\\WinBrew\\lib', 
            'C:\\WinBrew\\bin', 
        ])
        # Extra Windows includes

        self._setup_build()

        pchenv = self.env.Clone()
        pchenv.Append(CXXFLAGS='/Yc%s' % self.pch)
        self.pch = pchenv.StaticObject('build/src/Common', 'build/src/Common.cpp')

        self._finish_build()

    def _setup_unix(self):
        # Set up OS X/Linux-specific options
        if self.build_mode == 'debug':
            self.env.Append(CXXFLAGS='-O0')
        elif self.build_mode == 'release':
            self.env.Append(CXXFLAGS='-O2')
        else:
            assert not "Unknown build type"

        self.env['CXX'] = 'clang++'
        self.env.Append(CXXFLAGS='-std=c++11 -stdlib=libc++ -g -Wall -Werror -fPIC')
        for framework in self.frameworks:
            self.env.Append(LINKFLAGS='-framework %s' % framework)
        self.env.Append(LINKFLAGS='-stdlib=libc++')
        self.env.Append(BUILDERS={'Pch': Builder(action=build_pch)})
        self.src += self.env.Glob('build/src/**.s')
        # Add gas assembly files

        self._setup_build()

        pchenv = self.env.Clone()
        self.pch = pchenv.Pch('include/%s/Common.hpp.pch' % self.name, 'include/%s' % self.pch)
        self.env.Append(CXXFLAGS='-include include/%s/Common.hpp' % self.name)

        self._finish_build()

    def _setup_build(self):
        # Set up the basic build options
        self.libs = filter(self._lib_is_valid_for_platform, self.libs)
        self.libs = map(str, self.libs)

        self.env.Append(CPPDEFINES=self.defines)
        self.env.Append(CPPPATH=self.includes)
        self.env.Append(LIBPATH=self.lib_path)
        self.env.Append(LIBS=self.libs)

        self.src += self.env.Glob('build/src/**.cpp')
        self.src += self.env.Glob('build/src/**.c')
        self.src = filter(lambda x: 'Common.cpp' not in x.name, self.src)
        self.env.Depends(self.src, self.pch) # Wait for pch to build

    def _finish_build(self):
        if self.env['PLATFORM'] == 'win32':
            self.lib = self.env.StaticLibrary('lib/%s' % self.name, (self.src, self.pch))
        else:
            self.lib = self.env.SharedLibrary('lib/%s' % self.name, self.src)
        if self.kind == 'bin':
            main = self.env.Glob('Main.cpp')
            self.program = self.env.Program('bin/%s' % self.name, (self.lib, main))
        for tool in glob.glob('tools/*.cpp'):
            name = os.path.splitext(os.path.basename(tool.lower()))[0]
            self.env.Program('bin/%s-%s' % (self.name, name), (self.lib, tool))

    def _setup_tests(self):
        # Configure the test environment
        self.env.Append(BUILDERS={'Test': Builder(action=run_test)})
        self.tests = []
        testenv = self.env.Clone()
        testenv.Append(LIBS=self.lib)
        for test in self.env.Glob('build/test/**.cpp'):
            self.env.Depends(test, self.pch)
            name = test.name.replace('.cpp', '')
            if self.env['PLATFORM'] == 'win32':
                inputs = (test, self.pch)
            else:
                inputs = (test,)
            prog = testenv.Program('bin/test/%s' % name, inputs)
            if 'check' in COMMAND_LINE_TARGETS:
                self.tests.append(testenv.Test(name, prog))
        if 'check' in COMMAND_LINE_TARGETS:
            self.env.Alias('check', self.tests)

    def build(self):
        pass
        
        






