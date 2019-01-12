#!/bin/bash

#
# benchmark of shorts JSON 
#
export NDRX_BENCH_FILE=benchmark_short.txt
export NDRX_BENCH_CONFIGNAME="JSON buffers"

userregclt_json D || exit 1


export NDRX_BENCH_FILE=benchmark_string.txt
export NDRX_BENCH_CONFIGNAME="JSON buffers"

userregclt_json S || exit 2

