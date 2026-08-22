#include "ping.h"
#include "echo_packet.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 512

void ping(int fd, char destination_ip[], const char text[]) {
  size_t data_size = sizeof(*text);

  struct sockaddr_in addr;

  memset(&addr, 0, sizeof addr);

  addr.sin_family = AF_INET;
  if (inet_pton(AF_INET, destination_ip, &addr.sin_addr) <= 0) {
    fprintf(stderr, "Invlid destination IP address\n");
    return;
  }
  addr.sin_port = 0;

  if ((send_echo_message(fd, addr, text, data_size)) == -1) {
    int errnoc = errno;
    fprintf(stderr, "Failed to send echo message: %s\n", strerror(errnoc));
    return;
  }

  char reply_buf[BUF_SIZE];

  receive_echo_reply(fd, addr, reply_buf, BUF_SIZE);

  printf("%s\n", reply_buf);
}
