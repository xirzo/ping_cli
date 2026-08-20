
## Sources

- https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol

- https://en.wikipedia.org/wiki/Ping_(networking_utility)

- https://en.wikipedia.org/wiki/Network_layer

- http://www.iana.org/assignments/protocol-numbers/

- man icmp, raw(7), socket, bind

- RFC 792

- https://github.com/janwilmans/explain_icmp_ping/blob/main/src/c/ping.c#L83 (FOUND THIS AFTER 1.5 hours lul)

[Link](https://datatracker.ietf.org/doc/html/rfc792) to the document.

Echo or Echo Reply Message, Page 14.

```
0                   1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Type      |     Code      |          Checksum             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Identifier          |        Sequence Number        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Data ...
+-+-+-+-+-
```
