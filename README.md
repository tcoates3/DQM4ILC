# DQM4ILC
# Data Quality Monitoring for the International Linear Collider

## INSTALL:

Requires [ilcsoft](http://ilcsoft.desy.de/portal) and DQMCore installed
The install procedure is managed by [CMake](http://cmake.org)

All the needed packages are already installed in ILCSOFT, except [xdrlcio](https://github.com)

In the root directory :

```bash
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake [-DINSTALL_DOC=ON] ..
make
```

Note that the INSTALL_DOC option requires [doxygen](www.doxygen.org) installed to generate the code documentation

### Bug report

You can send emails to <dqm4hep@gmail.com>
or use the [github issues interface](https://github.Com/DQM4HEP/DQM4ILC/issues)
