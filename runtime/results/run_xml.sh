#!/bin/bash

#
# benchmark of shorts XML
#
export NDRX_BENCH_FILE=benchmark_short.txt
export NDRX_BENCH_CONFIGNAME="XML buffers"

userregclt_xml D || exit 1


export NDRX_BENCH_FILE=benchmark_string.txt
export NDRX_BENCH_CONFIGNAME="XML buffers"

userregclt_xml S || exit 2

