#!/bin/sh
arm-linux-gnueabihf-gcc *.c -o login.cgi -L. libcgic.so
