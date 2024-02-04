#!/usr/bin/env bash

mkdir -p build/
gcc hdlc_packet_parser.c yahdlc/C/yahdlc.c yahdlc/C/fcs.c -o build/hdlc_packet_parser
