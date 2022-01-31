# UDP_Send-Receive_on_Raspberry_Pi
 An experimental UDP Send/Receive Client for the Raspberry Pi

First attempt to create a UDP Client to communicate with an ESP8266 based remote sensor. The ESP8266 acts as the Server and simply returns whatever the Client sends to it.

Once a socket has been created, a single function 'SendRecv()' sends the UDP packet and then waits, with a timeout, for a reply.