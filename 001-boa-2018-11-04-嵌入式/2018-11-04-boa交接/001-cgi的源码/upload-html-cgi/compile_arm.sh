#!/bin/sh
arm-linux-gnueabihf-gcc *.c -o uploadFile.cgi -L. libcgic.so
