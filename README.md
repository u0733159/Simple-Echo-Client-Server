# Simple Echo Client and Server
This is the README file for this Simple TCP/IP based Echo Client and Server Project. This file contains the followings main parts: 
  - Group information and contributions
  - Application Usages
  - Application Architecture
  - Test Cases and Errata
  - Potential Improvements.
  - References
***
**1. Group Information**
  - Rui Liu implemented the server part and written function and readline function.
  - Jingze Sun did the client part and some tests.
 
**2. Application Usages**
  To run the application, you need to compile the whole programs using make, and then start the server program first and then the client program. 
    To run the server, use command: 
```sh
$ ./echoserver
```
to run the client, use command:
```sh
$ ./echoclient
```
Binary **echoserver** accepts 3 optional command line arguments: port number, backlog and usage instructions.
Binary **echoclient** accepts 3 optional command line arguments: server address, port number and usage instructions.

For example, we can run server with：
```sh
./echoserver -p 8988 
```
then we start the server at listening port 8988, the default pending backlog is 8, which means the server allows at most 8 connections at a time. Corresponding to the waiting server, server, we can run client with:
```sh
./echoclient -p 8988 -s 127.0.0.1
```
If the ip address is not specified, the client will choose to connect to the server with local Ip address.

**3. Application Architecture**
This client server echo model is based on TCP/IP with only IPv4 address specified. Optimization is done for the common read and write system calls. The updated readline function is about to handle the common issues which can happen in real network based applications. Readline reads a whole line each time, but the upper limit for the number of characters it can read is 2000 bytes as a defined value on variable ***BUFSIZE***. The written function wraps the regular write function with specified number of bytes as an argument. To make server be concurrent, the server use fork() system call to make a child process to handle the communication between the server the newly connected client. When the client a child process responsible for is disconnected, a message like "Child process <pid> terminated" will be sent to the initial server. This is done with signal system call.

**4. Test Cases and Errat**
* Here is a screenshot showing 3 clients get connected with the server separately:
![](connected.png?raw=true)

* Here is a screenshot showing 3 clients are communicating with server and server echos back the message it received.
![](communication.png?raw=true)

* Here is a screenshot showing a client disconnected from the server and the server gets know a client is disconnected:
![](disconnected_1.png?raw=true)

* Here is a screenshot showing all clients disconnected from the server was informed:
![](disconnected2.png?raw=true)

**5. Potential Improvments**


**6. References**



#### Building for source
For production release:
```sh
$ gulp build --prod
```
Generating pre-built zip archives for distribution:
```sh
$ gulp build dist --prod
```




**Free Software, Hell Yeah!**
