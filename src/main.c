#include "echo_packet.h"
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

  struct sockaddr_in addr;

  memset(&addr, 0, sizeof addr);
  addr.sin_family = AF_INET;
  if (inet_pton(AF_INET, DESTINATION_IP, &addr.sin_addr) <= 0) {
    fprintf(stderr, "Invalid IP address\n");
    return EXIT_FAILURE;
  }
  addr.sin_port = htons(0);

  char data[] = "Hello, World";
  size_t data_size = sizeof(data);
  size_t total_len = ICMP_HEADER_SIZE + data_size;

  unsigned char buffer[total_len];
  Echo_Message *msg = (Echo_Message *)buffer;

  init_echo_message(msg, data, data_size);
  if (sendto(fd, msg, total_len, 0, (struct sockaddr *)&addr, sizeof(addr)) ==
      -1) {
    perror("Failed to send packet");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
