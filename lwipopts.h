#ifndef LWIPOPTS_H
#define LWIPOPTS_H

/* Avoid redefinition of timeval */
#define LWIP_TIMEVAL_PRIVATE 0

/* Enable IP features */
#define LWIP_IPV4                   1
#define LWIP_IPV6                   0

/* Enable DHCP */
#define LWIP_DHCP                   1

/* Enable DNS */
#define LWIP_DNS                    1

/* Enable TCP and UDP */
#define LWIP_TCP                    1
#define LWIP_UDP                    1

/* Memory options */
#define MEM_SIZE                    10240   /* Adjust memory size for lwIP */

#define MEMP_NUM_TCP_PCB             5
#define MEMP_NUM_UDP_PCB             5

/* Enable the lwIP netconn and socket APIs */
#define LWIP_NETCONN                1
#define LWIP_SOCKET                 1

/* Set NO_SYS to 0 since you are using FreeRTOS */
#define NO_SYS                      0
#define SYS_LIGHTWEIGHT_PROT        1

#endif /* LWIPOPTS_H */