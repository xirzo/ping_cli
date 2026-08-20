#include "echo_packet.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

// (RFC 792 Page 14) Echo message format
// (RFC 792 Page 1) ICMP messages are sent using the basic IP header

#define DESTINATION_IP "192.168.31.92"

int main(void) {
  // I am 99% sure this is correct
  // http://www.iana.org/assignments/protocol-numbers/ for protocol
  int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (fd == -1) {
    int errnoc = errno;
    fprintf(stderr, "Failed to open a socket: %s\n", strerror(errnoc));
    return EXIT_FAILURE;
  }

  struct sockaddr_in addr;

  memset(&addr, 0, sizeof addr);
  addr.sin_family = AF_INET;
  // actually need to check for error (man says to use some other function)
  addr.sin_addr.s_addr = inet_addr(DESTINATION_IP);
  addr.sin_port = htons(0);

  Echo_Message msg;
  init_echo_message(&msg);
  if ((sendto(fd, (void *)&msg, sizeof msg, 0, (struct sockaddr *)&addr,
              sizeof addr)) == -1) {
    int errnoc = errno;
    fprintf(stderr, "Failed to send to the socket: %s\n", strerror(errnoc));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
