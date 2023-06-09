# NwnCharGen
Qt5 based character builder for Neverwinter Nights 2

# Building

* The project was built with Qt Creator using the mingw toolchain for Windows https://www.qt.io/download-open-source The crash reporter setup assumes mingw and may not work if built with a different toolchain.
* Parsing the NWN2 files relies on this fork https://github.com/rkibria/nwn2dev-public of SkywingvL's library. Point NWN2DATALIB_PATH to the path of the local checkout to build the rules generator.
* Apart from Qt the app has a dependency on the Boost library https://www.boost.org/ Set the appropriate CMake variables to link to it.

# Screenshots

![Overview tab](https://raw.githubusercontent.com/rkibria/NwnCharGen/master/images/screenshot1.png)

![Levels tab](https://raw.githubusercontent.com/rkibria/NwnCharGen/master/images/screenshot2.png)
