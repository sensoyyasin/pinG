#include "ft_ping.h"

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

void print_dns_error(int error_code);

void remove_protocol(char *url) {
        char *protocol_pos = strstr(url, "://");
        if (protocol_pos != NULL)
                memmove(url, protocol_pos + 3, ft_strlen(protocol_pos + 3) + 1);

        printf("            _        ____\n");
        printf("      _ __ (_)_ __  / ___|\n");
        printf("     | '_ \\| | '_ \\| |  _\n");
        printf("     | |_) | | | | | |_| |\n");
        printf("     | .__/|_|_| |_|\\____|\n");
        printf("     |_|                  \n");
}

// DNS resolution function
int resolve_dns(const char *hostname, struct addrinfo **result) {
    struct addrinfo hints;

    ft_memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_ICMP;
    //hints.ai_canonname = NULL;
    //hints.ai_addr = NULL;
    //hints.ai_next = NULL;

    return (getaddrinfo(hostname, NULL, &hints, result));
}

void print_resolved_ips(struct addrinfo *ai_result, char *dest_ip) {
    char addr_str[NI_MAXHOST];
    struct addrinfo *last_addr = ai_result;

    // IP adreslerini topla
    while (last_addr != NULL) {
        int ni_ret = getnameinfo(last_addr->ai_addr, last_addr->ai_addrlen, addr_str, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if (ni_ret != 0)
            print_dns_error(ni_ret);
        else {
            printf("Resolved IP address: %s\n", addr_str);
            strncpy(dest_ip, addr_str, NI_MAXHOST);
        }

        last_addr = last_addr->ai_next;
    }
}


void    print_dns_error(int error_code) {
    switch(error_code) {
        case EAI_FAMILY:
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
        case EAI_MEMORY:
            fprintf(stderr, "Error: Out of memory.\n");
            break;
        case EAI_NONAME:
            fprintf(stderr, "Error: The specified network host exists, but does not have any network addresses defined.\n");
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
