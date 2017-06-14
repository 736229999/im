#!/bin/sh
protoc --cpp_out=../../common --java_out=./ *.proto
