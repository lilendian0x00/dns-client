# dns-client
very simple IPv4 dns resolver (UDP &amp; TCP)

# Build instructions
#### CMake
    $ mkdir build && cd build
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    $ cmake --build . --config Release


# Run
    ./dns-client <dns_server> <domain> <TCP, UDP>