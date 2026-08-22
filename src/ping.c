#include "ping.h"
#include "echo_packet.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>

void ping(int fd, char destination_ip[], const char text[]) {
  size_t data_size = strlen(text);

  struct sockaddr_in addr;

  memset(&addr, 0, sizeof addr);

  addr.sin_family = AF_INET;
  if (inet_pton(AF_INET, destination_ip, &addr.sin_addr) <= 0) {
    fprintf(stderr, "Invalid destination IP address\n");
    return;
  }
  addr.sin_port = 0;

  if ((send_echo_message(fd, addr, text, data_size)) == -1) {
    int errnoc = errno;
    fprintf(stderr, "Failed to send echo message: %s\n", strerror(errnoc));
    return;
  }

  if ((receive_echo_reply(fd, addr)) == -1) {
    fprintf(stderr, "Failed to receive echo reply\n");
    return;
  }
}
