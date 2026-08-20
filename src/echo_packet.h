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

#define ICMP_ECHO_MESSAGE_TYPE 8
#define ICMP_ECHO_REPLY_TYPE 0
#define ICMP_ECHO_CODE 0

typedef struct {
  unsigned char type;
  unsigned char code;
  unsigned short checksum;
  unsigned short identifier;      // may be set to zero, as code is 0
  unsigned short sequence_number; // may be set to zero, as code is 0
  // not adding data, as it's optional
} Echo_Message;

void init_echo_message(Echo_Message *msg);

#endif // ECHO_PACKET_H
