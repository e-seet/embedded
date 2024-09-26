#ifndef LWIP_SOCKETS_H
#define LWIP_SOCKETS_H

#include <stddef.h>
#include <stdint.h>

/* Address families */
#define AF_INET   2  /* Internet IP Protocol */
#define AF_INET6 10  /* IP version 6 */

/* Socket types */
#define SOCK_STREAM 1   /* TCP */
#define SOCK_DGRAM  2   /* UDP */

/* Protocol families, same as address families */
#define PF_INET AF_INET

/* Generic socket option */
#define SOL_SOCKET  1

/* Flags for send/recv */
#define MSG_DONTWAIT 0x40  /* Non-blocking i/o for send and recv */

/* Socket-level options */
#define SO_REUSEADDR  2  /* Allow reuse of local addresses */

/* Special address for binding to all interfaces */
#define INADDR_ANY 0x00000000

/* Internet address (used with sockaddr_in) */
struct in_addr2 {
    uint32_t s_addr;  /* 32-bit IP address */
};

/* Socket address structure (IPv4) */
struct sockaddr_in {
    short          sin_family;   /* Address family (AF_INET) */
    uint16_t       sin_port;     /* Port number (16 bits) */
    struct in_addr2 sin_addr;     /* Internet address */
    char           sin_zero[8];  /* Padding */
};
typedef int socklen_t;

/* Functions */
int socket(int domain, int type, int protocol);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
int close(int sockfd);

/* Byte order conversion functions */
uint16_t htons(uint16_t hostshort);
uint32_t htonl(uint32_t hostlong);
uint16_t ntohs(uint16_t netshort);
uint32_t ntohl(uint32_t netlong);

#endif /* LWIP_SOCKETS_H */