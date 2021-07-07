#!python
#cython: language_level=3
import os
import sys
import glob
import subprocess
import pkg_resources
from distutils.core import setup
from distutils.extension import Extension


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
        cmd = [sys.executable, "-m", "pip", "install","-t", "setup-requires"] + to_install
        subprocess.call(cmd)
excludes = ["read.c"]
sources =[x for x in glob.glob('lib/raresim/src/*.c')  if not any(e in x for e in excludes)]
#sources.extend([x for x in glob.glob('lib/raresim/src/*.h') if not any(e in x for e in excludes)])
requires = ['cython']
install_requirements(missing_requirements(requires))


here = os.path.abspath(".")
from Cython.Distutils import build_ext
extension = [Extension(
    name="rareSim",
    sources=["rareSim.pyx"]  + sources,
    #libraries=["lists"],
    #library_dirs=["lib", ],
    include_dirs=[here,"lib/raresim/src/",]
)]

cmdclass = {'build_ext': build_ext}
setup(
    name="rareSim",
    description="python wrapper to Rare Simulation Projects",
    author= "PJSheini",
    cmdclass=cmdclass,
    ext_modules=extension,
)
