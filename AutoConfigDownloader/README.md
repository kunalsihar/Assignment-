
# AutoConfigDownloader

## Description
A UDP-based configuration management system for embedded Linux systems.

## Build Instructions
1. Navigate to the project directory:
    ```sh
    cd build
    cmake ..
    make
    ```

2. Run the server and client in separate terminals:
    ```sh
    ./server
    ./client
    ```

## Features
- Server validates requests and sends binary configuration files.
- Client retries on failures and saves the file locally.

## Requirements
- GCC
- CMake
- Linux environment
