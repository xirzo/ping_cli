#include "echo_packet.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

// The checksum is the 16-bit ones's complement of the one's
// complement sum of the ICMP message starting with the ICMP Type.
// For computing the checksum , the checksum field should be zero.
// If the total length is odd, the received data is padded with one
// octet of zeros for computing the checksum.  This checksum may be
// replaced in the future.

#define BUFFER_SIZE 1512

static unsigned short calculate_checksum(const void *data, size_t len) {
  const unsigned short *ptr = (const unsigned short *)data;
  unsigned int sum = 0;

  while (len > 1) {
    sum += *ptr++;
    len -= 2;
  }

  if (len == 1) {
    unsigned short odd_byte = 0;
    *(unsigned char *)(&odd_byte) = *(const unsigned char *)ptr;
    sum += odd_byte;
  }

  while (sum >> 16) {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }

  return (unsigned short)(~sum);
}

static void init_echo_message(Echo_Message *msg, const void *data,
                              size_t data_size) {
  size_t total_len = ICMP_HEADER_SIZE + data_size;

  msg->type = ICMP_ECHO_MESSAGE_TYPE;
  msg->code = ICMP_ECHO_CODE;
  msg->checksum = 0;
  msg->identifier = 0;
  msg->sequence_number = 0;

  if (data != NULL && data_size > 0) {
    memcpy(msg->data, data, data_size);
  }

  msg->checksum = calculate_checksum(msg, total_len);
}

int send_echo_message(int fd, struct sockaddr_in addr, const void *data,
                      size_t data_size) {

  size_t total_len = ICMP_HEADER_SIZE + data_size;

  unsigned char buffer[total_len];
  Echo_Message *msg = (Echo_Message *)buffer;
  init_echo_message(msg, data, data_size);

  if (sendto(fd, msg, total_len, 0, (struct sockaddr *)&addr, sizeof(addr)) ==
      -1) {
    return -1;
  }

  return 0;
}

int receive_echo_reply(int fd) {
  // SAFETY IS NOT GUARANTEED
  // 😄😄😄😄😄😄😄😄😄😄😄😄😄😄😄
  struct sockaddr_in addr;
  socklen_t peerlen = sizeof(addr);
  char buf[BUFFER_SIZE];
  ssize_t n = recvfrom(fd, buf, BUFFER_SIZE, 0, (struct sockaddr *)&addr, &peerlen);
  if (n == -1) {
    int errnoc = errno;
    fprintf(stderr, "Failed to receive data: %s\n", strerror(errnoc));
    return -1;
  }

  if (n < (ssize_t)sizeof(struct iphdr)) {
    fprintf(stderr, "Reply packet's size is less than IPv4 header\n");
    return -1;
  }

  struct iphdr *ip = (struct iphdr *)buf;
  if (n < (ssize_t)(ip->ihl * 4 + ICMP_HEADER_SIZE)) {
    fprintf(stderr, "Incorrect reply packet size\n");
    return -1;
  }
  int data_len = (int)(n - (ip->ihl * 4 + ICMP_HEADER_SIZE));

  Echo_Message *reply = (Echo_Message*)(buf + ip->ihl * 4);

  if (reply->type != ICMP_ECHO_REPLY_TYPE){
    fprintf(stderr, "Received a message, which is not a reply\n");
    return -1;
  }

  // TODO: validate checksum / identifier

  printf("%d\n", reply->type);
  printf("%.*s\n", data_len, (char*)reply->data);

  return 0;
}
