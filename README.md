# zmq-http
ZMQ HTTP client-server application

Build steps:

1. Build libzmq  make -j4 install

2. Build cppzmq  make -j4 install

3. Build main project 

Move text files to build directory.

Run ZMQHTTPServer

Run ZMQHTTPClient

Available commands for client:

/zmq - receives students from server using zeromq pub/sub protocol

/http - receives students from server using http

/stop - stops both client and server apps
