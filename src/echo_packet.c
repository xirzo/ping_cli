#include "echo_packet.h"
#include <string.h>

void init_echo_message(Echo_Message *msg) {
  memset(msg, 0, sizeof *msg);
  msg->type = ICMP_ECHO_MESSAGE_TYPE;
  msg->code = ICMP_ECHO_CODE;
  // TODO: calculate checksum
  // msg->checksum = ...
  msg->identifier = 0;
  msg->sequence_number = 0;
}
