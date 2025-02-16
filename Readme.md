# Network Adapter Listing Program

This program lists all available network adapters on a Unix-based system along with their respective IPv4 and IPv6 addresses, MAC addresses, interface status, subnet mask, and broadcast address. It also supports JSON output and filtering options.

## Features
- Lists all available network interfaces.
- Displays both IPv4 and IPv6 addresses.
- Shows MAC addresses.
- Indicates whether an interface is `UP` or `DOWN`.
- Prints subnet mask and broadcast address.
- Supports filtering by interface name or IP type (IPv4/IPv6).
- Provides JSON output mode for easier parsing.
- Saves output to a file when specified.
- Improved error handling for better debugging.

## Prerequisites
This program is intended to be compiled and executed on Unix-based systems (Linux, macOS). It will not work on Windows systems without additional compatibility layers.

## Compilation Instructions
To compile the program, use the GCC compiler:

```sh
gcc -o network_adapters network_adapters.c
```

## Running the Program
The program can be executed normally using:

```sh
./network_adapters
```

### Optional Arguments:
- `-j` or `--json`: Output results in JSON format.
- `-i <interface>`: Show details for a specific network interface.
- `-t ipv4 | ipv6`: Filter results to show only IPv4 or IPv6 addresses.
- `-o <filename>`: Save the output to a file.

#### Example Usage
```sh
./network_adapters -t ipv4 -o network_info.txt
./network_adapters -j > output.json
./network_adapters -i eth0
```

## Code Explanation

1. **Retrieving Network Interfaces:**
   - The function `getifaddrs(&addresses)` retrieves the list of network interfaces.
   - If this function fails, a detailed error message is printed.

2. **Iterating Through the List:**
   - The program loops through the linked list of interfaces.
   - It checks if the address family is `AF_INET` (IPv4) or `AF_INET6` (IPv6).
   - Extracts MAC addresses using `SIOCGIFHWADDR` ioctl.
   - Retrieves interface status using `ifa_flags`.
   - Extracts subnet mask (`ifa_netmask`) and broadcast address (`ifa_broadaddr`).

3. **Filtering and Output Options:**
   - Users can specify filters via command-line arguments.
   - JSON output mode formats data in a structured way.
   - Output can be saved to a file for later use.

4. **Freeing Memory:**
   - The dynamically allocated memory is released using `freeifaddrs(addresses)` at the end.

## Example Output
```sh
{
  "interfaces": [
    {
      "name": "eth0",
      "status": "UP",
      "ipv4": "192.168.1.2",
      "ipv6": "fe80::1a2b:3c4d:5e6f:7g8h",
      "mac": "00:1A:2B:3C:4D:5E",
      "netmask": "255.255.255.0",
      "broadcast": "192.168.1.255"
    }
  ]
}
```

## Notes
- This program is designed to run only on Unix-based systems.
- It requires the `<ifaddrs.h>`, `<netinet/in.h>`, `<sys/socket.h>`, and `<arpa/inet.h>` libraries.
- Ensure that the development tools (`Xcode Command Line Tools` on macOS) are installed if needed.

## License
This project is open-source and can be modified and distributed freely.

