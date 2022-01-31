// cleaned-up UDP send and receive client

// packet format: <id><seq><Bytes><chksum><----data---->

#include <iostream>
//#include <fstream>
#include <string.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

// server IP addresses
#define NODE_1  inet_addr("192.168.0.50")
#define NODE_2  inet_addr("192.168.0.51")
#define NODE_3  inet_addr("192.168.0.52")
#define NODE_4  inet_addr("192.168.0.53")
#define NODE_5  inet_addr("192.168.0.54")
// server port
#define PORT 50000

// recv timeout period
#define TIMEOUT_PERIOD  0,50000 // seconds,microsecinds

using namespace std;

enum {ID = 0,SEQ,BYTES,CHKSUM,DATA};

enum {TIME_ = 0,SEND_};

char buff[256] = {0};

// universal 'sendRecv - revcFrom' function
int sendRecv(int _socketfd, uint32_t _targetIP, uint16_t _port, char * _buff, int _buffSize, int _msgLen) {
  // create struct
  struct sockaddr_in serAddr;
  // configure the serAddr struct and target IP address and Port
  serAddr.sin_addr.s_addr = _targetIP;
  serAddr.sin_port = htons(_port);
  serAddr.sin_family = AF_INET;
  int result;
  // send the packet
  result = sendto(_socketfd, _buff, _msgLen, 0, (struct sockaddr*)&serAddr, (socklen_t) sizeof(serAddr));
  if(result < 0) return result;
  
  // clear the buffer
  memset(_buff,0,_buffSize);
  // receive an incoming packet on the current socket
  result = recv(_socketfd, _buff, _buffSize, 0);
  //cout << result << endl;
  return result;
}


// rough 64bit millisecond count
uint64_t millis(bool set = false)
{
  static uint64_t startMillis;
  uint64_t now;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);
  if(set) {
    startMillis = now;
    set = false;
  }
return (now - startMillis);
}

int main(int argc, char** argv) {
  millis(true);
  int socketfd;
  
  //create a UDP socket
  if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation error...\n");
    exit(-1);
  }
  
  // modify the default timeout for receiving
  struct timeval sockTimeout={TIMEOUT_PERIOD}; //(sec,usec) set timeout for 1 second
  setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &sockTimeout, sizeof(sockTimeout));

for(;;) {
  // send a packet
  srand(time(0));
 
  sprintf(buff,"%d",rand());
  int readStatus = sendRecv(socketfd, NODE_1, PORT, buff, sizeof(buff), strlen(buff));
  if(readStatus < 0) perror("sending error...\n");
  
  printf("%s\t%i\n",buff,readStatus);
  
  sleep(1);
}
  close(socketfd);
  return 0;
}
