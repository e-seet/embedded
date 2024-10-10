#ifndef LWIPOPTS_H
#define LWIPOPTS_H

/* Define this to match your system requirements */
#define LWIP_USE_TCP                1
#define LWIP_USE_UDP                1
#define LWIP_DHCP                   1
#define LWIP_ICMP                   1
#define LWIP_DNS                    1

/* Memory options */
#define MEM_SIZE                    (10240) /* Adjust memory size */

/* TCP options */
#define TCP_MSS                     (1500)
#define TCP_SND_BUF                 (3 * TCP_MSS)
#define TCP_WND                     (2 * TCP_MSS)

/* Enable lwIP APIs */
#define LWIP_NETCONN                1
#define LWIP_SOCKET                 1
#define LWIP_DHCP                   1
#define LWIP_DNS                    1

/* Set NO_SYS to 0 since you're using FreeRTOS */
#define NO_SYS                      0
#define SYS_LIGHTWEIGHT_PROT        1

/* Enable debug if necessary */
#define LWIP_DEBUG                  0

#endif /* LWIPOPTS_H */