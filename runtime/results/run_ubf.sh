#!/bin/bash

#
# benchmark of shorts UBF
#
export NDRX_BENCH_FILE=benchmark_short.txt
export NDRX_BENCH_CONFIGNAME="UBF buffers"

userregclt_ubf D || exit 1


export NDRX_BENCH_FILE=benchmark_string.txt
export NDRX_BENCH_CONFIGNAME="UBF buffers"

userregclt_ubf S || exit 2

