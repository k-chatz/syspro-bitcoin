cmake_minimum_required(VERSION 3.13)
project(bitcoin C)

set(CMAKE_C_STANDARD 99)

add_executable(bitcoin main.c list.c hash.c wallet.c bitcoin.c transaction.c queue.c)