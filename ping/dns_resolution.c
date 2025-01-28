#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

/*
struct addrinfo {
        int              ai_flags; 
        int              ai_family; - AFINET IPv4
        int              ai_socktype; - SOCK_STREAM , SOCK_RAW
        int              ai_protocol;
        socklen_t        ai_addrlen;
        struct sockaddr *ai_addr;
        char            *ai_canonname;
        struct addrinfo *ai_next;
    };
*/

// DNS resolution function
int resolve_dns(const char *hostname, struct addrinfo **result) {
    struct addrinfo hints;

    ft_memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    return (getaddrinfo(hostname, "80", &hints, result));
}

// Print the resolved IP addresses
void print_resolved_ips(struct addrinfo *ai_result) {
    char addr_str[NI_MAXHOST];
    struct addrinfo *ai_tmp;

    for (ai_tmp = ai_result; ai_tmp != NULL; ai_tmp = ai_tmp->ai_next) {
        int ni_ret = getnameinfo(ai_tmp->ai_addr, ai_tmp->ai_addrlen, addr_str, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if (ni_ret != 0) {
            print_dns_error(ni_ret);
            freeaddrinfo(ni_ret);
            continue;
        }

        printf("Resolved IP address: %s\n", addr_str);
    }
}

void    print_dns_error(int error_code) {
    switch(error_code) {
        case EAI_ADDRFAMILY:
            fprintf(stderr, "Error: The specified network host does not have any network addresses...\n");
            break;
        case EAI_AGAIN:
            fprintf(stderr, "Error: Temporary failure in name resolution. Try again later.\n");
            break;
        case EAI_BADFLAGS:
            fprintf(stderr, "Error: Invalid flags in hints.ai_flags.\n");
            break;
        case EAI_FAIL:
            fprintf(stderr, "Error: Permanent failure in name resolution.\n");
            break;
        case EAI_FAMILY:
            fprintf(stderr, "Error: The requested address family is not supported.\n");
            break;
        case EAI_MEMORY:
            fprintf(stderr, "Error: Out of memory.\n");
            break;
        case EAI_NODATA:
            fprintf(stderr, "Error: The specified network host exists, but does not have any network addresses defined.\n");
            break;
        case EAI_NONAME:
            fprintf(stderr, "Error: The node or service is not known.\n");
            break;
        case EAI_SERVICE:
            fprintf(stderr, "Error: The requested service is not available for the requested socket type.\n");
            break;
        case EAI_SOCKTYPE:
            fprintf(stderr, "Error: The requested socket type is not supported.\n");
            break;
        case EAI_SYSTEM:
            perror("System error: ");
            break;
        default:
            fprintf(stderr, "Unknown error: %s\n", gai_strerror(error_code));
    }
}