#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <json-c/json.h>

#define MAC_STR_LEN 18

void get_mac_address(const char *iface, char *mac_str) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        strcpy(mac_str, "N/A");
        return;
    }

    struct ifreq ifr;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
        unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
        snprintf(mac_str, MAC_STR_LEN, "%02X:%02X:%02X:%02X:%02X:%02X",
                 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    } else {
        strcpy(mac_str, "N/A");
    }

    close(fd);
}

void print_adapter_info(struct ifaddrs *address, int json_output, struct json_object *json_array) {
    int family = address->ifa_addr->sa_family;
    if (family == AF_INET || family == AF_INET6) {
        char ip[100], netmask[100], broadcast[100] = "N/A";
        const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
        
        getnameinfo(address->ifa_addr, family_size, ip, sizeof(ip), 0, 0, NI_NUMERICHOST);
        getnameinfo(address->ifa_netmask, family_size, netmask, sizeof(netmask), 0, 0, NI_NUMERICHOST);

        if (address->ifa_broadaddr)
            getnameinfo(address->ifa_broadaddr, family_size, broadcast, sizeof(broadcast), 0, 0, NI_NUMERICHOST);

        char mac_address[MAC_STR_LEN] = {0};
        get_mac_address(address->ifa_name, mac_address);

        int status = (address->ifa_flags & IFF_UP) ? 1 : 0;

        if (json_output) {
            struct json_object *iface_json = json_object_new_object();
            json_object_object_add(iface_json, "name", json_object_new_string(address->ifa_name));
            json_object_object_add(iface_json, "status", json_object_new_string(status ? "UP" : "DOWN"));
            json_object_object_add(iface_json, "ip", json_object_new_string(ip));
            json_object_object_add(iface_json, "netmask", json_object_new_string(netmask));
            json_object_object_add(iface_json, "broadcast", json_object_new_string(broadcast));
            json_object_object_add(iface_json, "mac", json_object_new_string(mac_address));
            json_object_array_add(json_array, iface_json);
        } else {
            printf("%s\t%s\t%s\t%s\t%s\t%s\n", address->ifa_name, status ? "UP" : "DOWN",
                   family == AF_INET ? "IPv4" : "IPv6", ip, netmask, broadcast);
        }
    }
}

int main(int argc, char *argv[]) {
    struct ifaddrs *addresses;
    if (getifaddrs(&addresses) == -1) {
        perror("getifaddrs call failed");
        return EXIT_FAILURE;
    }

    int json_output = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-j") == 0 || strcmp(argv[i], "--json") == 0) {
            json_output = 1;
            break;
        }
    }

    if (!json_output) {
        printf("Interface\tStatus\tType\tIP Address\tNetmask\tBroadcast\n");
    }

    struct json_object *json_array = json_object_new_array();
    struct ifaddrs *address = addresses;

    while (address) {
        print_adapter_info(address, json_output, json_array);
        address = address->ifa_next;
    }

    if (json_output) {
        printf("%s\n", json_object_to_json_string_ext(json_array, JSON_C_TO_STRING_PRETTY));
    }

    json_object_put(json_array);
    freeifaddrs(addresses);
    return EXIT_SUCCESS;
}
