#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string_view>
#include <string>
#include <sys/socket.h>

// (RFC 792 Page 14) Echo message format
// (RFC 792 Page 1) ICMP messages are sent using the basic IP header

// constexpr std::string_view INTERFACE_IP = "192.168.31.90";
constexpr std::string DESTINATION_IP = "192.168.31.92";
int main() {
  // I am 99% sure this is correct
  // http://www.iana.org/assignments/protocol-numbers/ for protocol
  int fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (fd == -1) {
    int errnoc = errno;
    std::cerr << "Failed to open a socket: " << strerror(errnoc) << std::endl;
    return EXIT_FAILURE;
  }

  sockaddr_in addr;

  std::memset(&addr, 0, sizeof addr);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(DESTINATION_IP.data());
  addr.sin_port = htons(0); 

  char buf[] = "Hello, World!";

  if ((sendto(fd, buf, sizeof buf, 0, (struct sockaddr *)&addr, sizeof addr)) == -1) {
    int errnoc = errno;
    std::cerr << "Failed to send to the socket: " << strerror(errnoc)
              << std::endl;
    return EXIT_FAILURE;
  }

  // sockaddr_in addr;
  // std::memset(&addr, 0, sizeof addr);
  // if ((inet_aton(INTERFACE_IP.data(), &addr.sin_addr)) == 0) {
  //   std::cerr << "Failed to convert interface address to network byte order"
  //             << std::endl;
  //   return EXIT_FAILURE;
  // }
  // addr.sin_family = AF_INET;
  // addr.sin_port = IPPROTO_ICMP;

  // maybe I don't have to bind???
  // if ((bind(fd, (struct sockaddr *)&addr, sizeof addr)) == -1) {
  //   int errnoc = errno;
  //   std::cerr << "Failed to bind the socket: " << strerror(errnoc) <<
  //   std::endl; return EXIT_FAILURE;
  // }

  return EXIT_SUCCESS;
}
