// // #ifndef LWIPOPTS_H
// // #define LWIPOPTS_H

// // #define NO_SYS 1
// // #define LWIP_SOCKET 0
// // #define MEMP_MEM_MALLOC 1
// // #define LWIP_NETCONN 0
// // #define LWIP_RAW 1
// // #define LWIP_IPV6 0

// // #endif // LWIPOPTS_H

// #ifndef LWIPOPTS_H
// #define LWIPOPTS_H

// // Enable systemless mode
// #define NO_SYS 1
// // #define NO_SYS 0

// // Enable lwIP sockets
// #define LWIP_SOCKET 1

// // Enable memory allocation using standard malloc
// #define MEMP_MEM_MALLOC 1

// // Disable Netconn API (since we use sockets)
// #define LWIP_NETCONN 0

// // Enable raw API (optional, depending on your needs)
// #define LWIP_RAW 1

// // Disable IPv6 if you're only using IPv4 (you can enable if you need it)
// #define LWIP_IPV6 0

// #endif // LWIPOPTS_H

#define NO_SYS 1
#define LWIP_SOCKET 1
#define MEMP_MEM_MALLOC 1
#define LWIP_NETCONN 0
#define LWIP_RAW 1
#define LWIP_IPV6 0