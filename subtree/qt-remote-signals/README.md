qt-remote-signals
=================

Creates ready-to use client and server classes to transfer signals over network using QIODevice and QDataStream

Usage
-----

You need qt and qt creator.
Open source/qrRemoteSignals.pro with qt creator and compile the binary (the binary is created in the `bin` folder)

There is a test definition file in test/TestData.signals, look into this and review the content.

To create the Client and Server files, go into the `bin` directory and run `qtRemoteSignals ../test/TestData.signals ../test`.
The first param defines the signal definition file, the second one defines the output directory.
The command will create the Files `TestDataClient.h`, `TestDataClient.cpp`, `TestDataServer.h` and `TestDataServer.cpp` int the `test` folder.

You can then build the Client and Server projects in the `test` folder. 
Then, first run the server. Afterwards run the client and connect to the server within 30 seconds.

Theory
------

The signals are transmitted over a QIODevice, thus a QLocalServer, QTcpServer or QBuffer can be used. 
However, only QIODevice instances that emit readyRead can be used. Therefore, this implementation CAN NOT be used with QFile, and thus cannot be used on stdio.

On the client side, the signals are implemented as slots, wrapping the parameters into a QDataStream and writing them on the QIODevice. 
The server side listens to readyRead and emits the signals to the server. Before signals are emmited, you need to call the `initialize()` method.
Every signal transmission includes the QtRemoteSignal implementation version and a md5 checksum of the signal definition file to avoid incompatible builds working together.
If an incompatible build (different version of the .signal file) is detected, the receiving side quits with a `qFatal(...)`.

You can use every type that can be put into a QDataStream. All basic and all simple qt types (such as QString, QByteArray ...) work out of the box.
If you need more complex data transmitted, you need to define the `QDataStream& operator<<( ... )` and `QDataStream& operator>>( ... )` methods for that type, and include the definition file.
You can use `#include` in the signal definition file. 

There are currently no sanity checks for the definition file. So it it does not work as expected, please check your .signal file first. 
Also, you might need to include some qt types if you use them in your signals.

License
-------
The code is licensed under GPL v3.0 or later (see attached `LICENSE` file)

Author
------
Claas Diederichs (claas.diederichs at uni-oldenburg.de)
