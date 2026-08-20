#include "echo_packet.h"
#include <arpa/inet.h>
#include <stddef.h>
#include <string.h>

// The checksum is the 16-bit ones's complement of the one's
// complement sum of the ICMP message starting with the ICMP Type.
// For computing the checksum , the checksum field should be zero.
// If the total length is odd, the received data is padded with one
// octet of zeros for computing the checksum.  This checksum may be
// replaced in the future.

// https://www.tutorialspoint.com/article/c-program-to-implement-checksum
// https://stackoverflow.com/questions/52980000/calculating-a-16-bit-checksum
static unsigned short calculate_checksum(const char *data, size_t len) {
  if (len == 0) {
    return 0;
  }

  const unsigned char *ptr = (const unsigned char *)data;
  size_t even_len = len - len % 2;

  unsigned int chsm = 0;

  size_t i;
  for (i = 0; i < even_len; i += 2) {
    chsm += ((unsigned int)ptr[i] << 8) | (unsigned int)ptr[i + 1];
  }

  if (i < len) {
    chsm += ((unsigned int)ptr[i] << 8);
  }

  while (chsm >> 16) {
    chsm = (chsm & 0xFFFF) + (chsm >> 16);
  }

  return (unsigned short)(~chsm);
}

void init_echo_message(Echo_Message *msg) {
  memset(msg, 0, sizeof *msg);
  msg->type = ICMP_ECHO_MESSAGE_TYPE;
  msg->code = ICMP_ECHO_CODE;
  msg->identifier = 0;
  msg->sequence_number = 0;
  msg->checksum = htons(calculate_checksum((char *)msg, sizeof *msg));
}
