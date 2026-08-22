#include "ping.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define DESTINATION_IP "192.168.31.92"

int main(void) {
  // http://www.iana.org/assignments/protocol-numbers/ for protocol
  int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (fd == -1) {
    int errnoc = errno;
    fprintf(stderr, "Failed to open a socket: %s\n", strerror(errnoc));
    return EXIT_FAILURE;
  }

  ping(fd, DESTINATION_IP, "Hello, World");

  return EXIT_SUCCESS;
}
