#ifndef ECHO_PACKET_H
#define ECHO_PACKET_H

// Page 14
// https://www.rfc-editor.org/info/rfc792/

//  0                   1                   2                   3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |     Type      |     Code      |          Checksum             |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Identifier          |        Sequence Number        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |     Data ...
// +-+-+-+-+-

#include <netinet/in.h>
#include <stddef.h>
#include <sys/types.h>

#define ICMP_ECHO_MESSAGE_TYPE 8
#define ICMP_ECHO_REPLY_TYPE 0
#define ICMP_ECHO_CODE 0
#define ICMP_HEADER_SIZE 8

typedef struct {
  unsigned char type;
  unsigned char code;
  unsigned short checksum;
  unsigned short identifier;      // may be set to zero, as code is 0
  unsigned short sequence_number; // may be set to zero, as code is 0
  unsigned char data[];
} __attribute__((packed)) Echo_Message;

// data maybe set set to null. returns -1 on failure and sets errno
int send_echo_message(int fd, struct sockaddr_in addr, const void *data,
                      size_t data_size);

ssize_t receive_echo_reply(int fd, struct sockaddr_in addr, void *buf,
                           size_t buf_size);

#endif // ECHO_PACKET_H
