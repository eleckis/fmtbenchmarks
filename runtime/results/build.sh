#!/bin/bash

#
# @(#) Run all bencharmsk and build a charts
#

rm *.txt
rm *.png
# Run XML Bench
./run_xml.sh || exit 1

export NDRX_BENCH_FILE="benchmark_short.txt"
export NDRX_BENCH_TITLE="Middleware message format benchmark (dec)"
export NDRX_BENCH_X_LABEL="Number of payload decimals"
export NDRX_BENCH_Y_LABEL="Milliseconds per 10000 msgs"
export NDRX_BENCH_OUTFILE="benchmark_short.png"

R -f genchart.r


export NDRX_BENCH_FILE="benchmark_string.txt"
export NDRX_BENCH_TITLE="Middleware message format benchmark (str)"
export NDRX_BENCH_X_LABEL="Number of payload strings"
export NDRX_BENCH_Y_LABEL="Milliseconds per 10000 msgs"
export NDRX_BENCH_OUTFILE="benchmark_string.png"

R -f genchart.r


