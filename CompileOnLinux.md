# Programs #

You will need a somewhat recent version of the g++ compiler suite, make and cmake to compile fluorescence.

# Libraries #

The following libraries can be installed using your distros package management tool:
  * boost (1.46 or newer): date\_time, filesystem, program\_options, system, thread, python
  * icu (4.4 or newer)
  * xcursor
  * curl
  * zlib
  * python
  * libsvn (1.6, **not** 1.7) (required for updater)

Additionally, the client uses the [fmod library](http://www.fmod.org) 4.40 for sound output. You can download the fmod SDK [here](http://www.fmod.org/index.php/download). After a "make install", the library is ready for use.

# ClanLib #
fluorescence is based on the [clanlib SDK](http://clanlib.org) **2.3.6**. Please download the source code from their homepage and apply the patch in the 3rdparty folder.
Compile clanlib according to the instructions on their homepage. Don't forget to "make install".

# Compilation #
Create a folder "build" in your fluorescence directory. Then, go to "src" (**not** build/src/) and execute either "make debug" or "make release".
You should find the fluorescence executable in the main directory if everything worked out.