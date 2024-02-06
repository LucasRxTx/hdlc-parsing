#!/usr/bin/env bash

mkdir -p build/
gcc main.c yahdlc/C/yahdlc.c yahdlc/C/fcs.c -o build/hdlc_packet_parser
