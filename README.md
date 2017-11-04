# libopenikev2
Library providing the core IKEv2 funcionability. It controls state machine, message parsing, message construction, SA management and all the IKEv2 processing.

## How to build
```
autoreconf -fvi
./configure && make
make install # as root
```
Needs `automake`, `autoconf`, and `libtool` to be installed.
