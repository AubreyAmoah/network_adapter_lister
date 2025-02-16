# Network Adapter Listing Program

This program lists all available network adapters on a Unix-based system along with their respective IPv4 and IPv6 addresses.

## Description

The provided C code uses the `getifaddrs` function to retrieve a linked list of network interfaces and their addresses. It then iterates through the list and prints out each network adapter's name, IP address type (IPv4 or IPv6), and the corresponding address.

## Features
- Lists all available network interfaces.
- Displays both IPv4 and IPv6 addresses.
- Uses `getnameinfo` to convert the address into a human-readable string.

## Prerequisites
This program is intended to be compiled and executed on Unix-based systems (Linux, macOS). It will not work on Windows systems without additional compatibility layers.

## Compilation Instructions
To compile the program, use the GCC compiler:

```sh
gcc -o network_adapters network_adapters.c
```

## Running the Program
Once compiled, execute the program using:

```sh
./network_adapters
```

## Code Explanation

1. **Retrieving Network Interfaces:**
   - The function `getifaddrs(&addresses)` retrieves the list of network interfaces.
   - If this function fails, an error message is printed.

2. **Iterating Through the List:**
   - The program loops through the linked list of interfaces using a `while` loop.
   - It checks if the address family is `AF_INET` (IPv4) or `AF_INET6` (IPv6).

3. **Extracting and Displaying Information:**
   - Prints the network adapter name.
   - Prints whether the address is IPv4 or IPv6.
   - Uses `getnameinfo` to extract and print the IP address in human-readable form.

4. **Freeing Memory:**
   - The dynamically allocated memory is released using `freeifaddrs(addresses)` at the end.

## Example Output
```sh
eth0    IPv4    192.168.1.2
lo      IPv4    127.0.0.1
wlan0   IPv6    fe80::1a2b:3c4d:5e6f:7g8h
```

## Notes
- This program is designed to run only on Unix-based systems.
- It requires the `<ifaddrs.h>`, `<netinet/in.h>`, and `<sys/socket.h>` libraries, which are available on Unix-based platforms.
- If running on macOS, ensure that the development tools (`Xcode Command Line Tools`) are installed.

## License
This project is open-source and can be modified and distributed freely.

