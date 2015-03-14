

# Getting source #
For people who never worked with git recommended to install [TortoiseGit](http://code.google.com/p/tortoisegit/). It is easy tool with GUI, which building in explorer and iterface same as TortoiseSvn. If you install it just click right mouse button on folder and select "Git Clone" and use git server url "https://code.google.com/p/fluorescence" to extract source code.


# Compilers #
At this moment project file only for MVS2010 exists in the repository, so it is recommend using "Microsoft Visual Studio 2010". There is free version of this IDE called "[Microsoft Visual C++ 2010 Express](http://www.microsoft.com/visualstudio/en-us/products/2010-editions/visual-cpp-express)".


# Deploying project #
To be able for building project you must to deploy it first and configure enviroment. To do soo you have to extract content of [winbuild.tar.bz2](http://fluorescence.googlecode.com/files/winbuild.tar.bz2) to ./winbuild/ directory, than run ./winbuild/debloy.bat and follow the instructions. This action will configure your output _build directory, added all nessesary headers, libraries, binaries and other files for building and runing application. It is the most easy way, if you do this it is not necessary for you to read "Manual deploying" section._


## Manual deploying ##

### Libraries ###
  * The **multithreaded, static runtime** version of the following boost 1.46 (or newer) libraries: date\_time, filesystem, program\_options, system, thread. You can install them using the download tool provided at http://www.boostpro.com/download/
  * ICU 4.4 (or newer) binaries from http://site.icu-project.org/
  * curl from http://curl.haxx.se/latest.cgi?curl=win32-devel-msvc
  * fmod ex 4.40 from http://www.fmod.org/fmod-downloads.html#FMODExProgrammersAPI
  * win32svn 1.6 (**not 1.7**) dev package from http://sourceforge.net/projects/win32svn/ (required for the updater)

### ClanLib ###
fluorescence is based on the [clanlib SDK](http://clanlib.org) **2.3.6**. Please download the source code from their homepage and apply the patch in the 3rdparty folder. Compile clanlib according to the instructions on their homepage. This will require a few additional dependency libraries.

### Set up library and include directory ###
The VS project provided with the source code relies on the availability of all required libs and headers in a folder **winbuild** in the root project directory. Create this folder and two subdirectories **include** and **lib32**.
> Copy the include files from boost, icu, clanlib and the clanlib dependencies to winbuild/include.
> Then, copy the boost, icu, clanlib and clanlib dependency libraries flat into to winbuild/lib32.
> Extract the libsvn\_XX, apr/libapr-1.lib and apr-util/libaprutil-1.lib from the win32svn package to winbuild/lib32; extract the includes to winbuild/include
> Check the example paths below if you are not sure where to put what.

> If the environment is set up correctly, the following files should exist under the given path:
    * winbuild/include/zlib.h
    * winbuild/include/boost/weak\_ptr.h
    * winbuild/include/ClanLib/core.h
    * winbuild/include/unicode/unistr.h
    * winbuild/include/apr/apr.h
    * winbuild/lib32/clanCore-static-mt.lib
    * winbuild/lib32/icudt.lib
    * winbuild/lib32/libboost\_date\_time-vc100-mt-s-1\_47.lib (exact name depends on boost version)
    * winbuild/lib32/libapr-1.lib

You also need to add the 32bit library directory of the DirectX SDK to your linker directories. Currently, the path included there is "C:\Program Files\Microsoft DirectX SDK %28June 2010%29\Lib\x86", but that might differ on your system.



# Compilation #
Currently, only the release build is tested in MVS2010. You can build project from console without running MVS, to do it run ./winbuild/build.bat.
If everything works, you should find a fluorescence\_win32.exe in the _build folder (  {SolutionDirectory}/../../../../_build/  ).


# winbuild git tag #
The major part of fluorescence development takes place on Linux. Thus, the project file for VS in the master branch is broken more often then not due to missing (new) files or added dependencies. There is, however, a git tag called "winbuild" that points to the last version tested on Windows. Using this tag gives you an easy start on Windows & VS, but does not include the latest features.
If you can't find the tag in your local clone, execute "git fetch --tags".