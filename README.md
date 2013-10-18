qt-remote-signals
=================

Creates ready-to use client and server classes to transfer signals over network using QIODevice and QDataStream

usage
-----

You need qt and qt creator.
open source/qrRemoteSignals.pro with qt creator and compile the binary (the binary is created in the `bin` folder)

There is a test definition file in test/TestData.signals, look into this and review the content

The create the Client and Server files, go into the bin directory and run `qtRemoteSignals ../test/TestData.signals ../test`
The first param defines the signal definition file, the second one defines the output directory

You can then build the Client and Server project in the `test` folder.

license
-------
The code is licensed under GPL v3.0 (see attached `LICENSE` file)

author
------
Claas Diederichs (claas.diederichs at uni-oldenburg.de)
