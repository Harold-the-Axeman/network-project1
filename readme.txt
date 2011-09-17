################################################################################
# readme.txt                                                                   #
#                                                                              #
# Description: A brief description of current implementation of lisod,         #
#              the select()-based echo server.                                 #
#                                                                              #
# Authors: Kaili Li <kailili@andrew.cmu.edu>                                   #
#                                                                              #
################################################################################

[TOC-1] Table of Contents
--------------------------------------------------------------------------------

        [TOC-1] Table of Contents
        [DES-2] Description of lisod
		[LOP-3] Limitation of the Program
		[RUN-4] Usage of lisod

		
[DES-2] Description of Files
--------------------------------------------------------------------------------
The lisod is implemented by using the linux select() function. And it works well
with the test script cp1_cheker.py. 


1. Use two FD_SET(readfds, writefds) to monitor the read/write request from client
, and also the connection request from client.
   1.1 Put all the active connection into the readfds to wait for send request.
   1.2 Put an active connection into the writefds when the connection just receive
   a send request.
       After an active connection response the request, clear it from the writefds,
   for active connections are always ready for write.
   
2. The buffer for the client is implemented as a data array.
   1.1 When an active connection need a buffer to receive, get one for it.
   1.2 But after that, it should send back the message before it receive another 
   message. It is a limitation in the code. But it is easy to implement.
   
		
[LOP-3] Limitation of lisod
-------------------------------------------------------------------------------

The following are the limitation of the current version server:

1. Each active client has just one buffer. It cannot receive twice before it
   send one.
   
2. The buffer for each active client is a fixed hard-coded value. Below is the
   definition of the value in the C code.
   #define BUFFER_SIZE 9999		

   
[RUN-4] How to Run
--------------------------------------------------------------------------------

Building and executing the server code should be very simple:

        make
        ./lisod &

To shut down it:
        ps -x |grep lisod
		kill 

The echo server has a serving port of 9999.
The DEBUG macro can be undefined if you do not want the lisod to print debug 
information.
