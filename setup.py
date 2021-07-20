#!python
#cython: language_level=3
import os
import sys
import glob
import subprocess
import pkg_resources
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

sys.path[0:0] = ['setup-requires']
pkg_resources.working_set.add_entry('setup-requires')


def missing_requirements(specifiers):
    for specifier in specifiers:
        try:
            pkg_resources.require(specifier)
        except pkg_resources.DistributionNotFound:
            yield specifier


def install_requirements(specifiers):
    to_install = list(specifiers)
    if to_install:
        cmd = [sys.executable, "-m", "pip", "install", "-t", "setup-requires"] + to_install
        subprocess.call(cmd)


excludes = ["read.c"]
sources = [x for x in glob.glob('lib/raresim/src/*.c')if not any(e in x for e in excludes)]
sources.extend([x for x in glob.glob('lib/zlib-1.2.11/*.c') if not any(e in x for e in excludes)])
requires = ['cython']
install_requirements(missing_requirements(requires))
here = os.path.abspath(".")
if not os.path.exists("lib/zlib-1.2.11/zconf.h"):
    import subprocess as sp
    sp.check_call("cd lib/zlib-1.2.11 && ./configure && make && make install", shell=True)


extension = [Extension(
    name="rareSim",
    sources=["rareSim.pyx"] + sources,
    libraries=['z', 'dl', 'm', 'curl', 'crypto', 'bz2', 'lzma'],
    include_dirs=[here, "lib/raresim/src/", "lib/zlib-1.2.11/", ]
)]

cmdclass = {'build_ext': build_ext}
setup(
    name="rareSim",
    description="python wrapper to Rare Simulation Projects",
    #dependency_links=['https://github.com/madler/zlib'],
    author="PJSheini",
    cmdclass=cmdclass,
    ext_modules=extension,
    )
