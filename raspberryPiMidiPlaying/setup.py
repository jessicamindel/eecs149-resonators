from distutils.core import setup, Extension
import sysconfig

def main():
    CFLAGS = ['-g', '-Wall', '-std=c99', '-fopenmp', '-pthread', '-O3','-lfluidsynth']
    LDFLAGS = ['-lfluidsynth']
    # Use the setup function we imported and set up the modules.
    # You may find this reference helpful: https://docs.python.org/3.6/extending/building.html
    # TODO: YOUR CODE HERE
    module1 = Extension('songbird', sources = ['songbird.c'], extra_link_args=LDFLAGS)
    setup(name = 'songbird', version = '1.0', description = 'This is the songbird Package', ext_modules = [module1])

if __name__ == "__main__":
    main()
