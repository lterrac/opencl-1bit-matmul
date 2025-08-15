#!/bin/bash

# Script to generate test matrices and ground truth
# Usage: ./generate_tests.sh <number_of_tests> <M> <N> <K>

set -e  # Exit on any error

if [ $# -ne 4 ]; then
    echo "Usage: $0 <number_of_tests> <M> <N> <K>"
    echo "This script generates the specified number of test cases."
    echo "  M: Number of rows in matrix A and output C"
    echo "  N: Number of columns in matrix B and output C"
    echo "  K: Shared dimension (columns of A, rows of B)"
    exit 1
fi

NUM_TESTS=$1
M=$2
N=$3
K=$4

# Check if executable exists
if [ ! -f "./build/generateTest" ]; then
    echo "Error: generateTest executable not found. Please build the project first."
    exit 1
fi

# Find the highest existing test ID
HIGHEST_ID=0
if [ -d "./tests" ]; then
    for test_dir in ./tests/test_*; do
        if [ -d "$test_dir" ]; then
            test_id=$(basename "$test_dir" | sed 's/test_//')
            if [ "$test_id" -gt "$HIGHEST_ID" ]; then
                HIGHEST_ID=$test_id
            fi
        fi
    done
fi

echo "Found highest test ID: $HIGHEST_ID"
echo "Will generate tests starting from ID: $((HIGHEST_ID + 1))"

# Create tests directory if it doesn't exist
mkdir -p ./tests

echo "Generating $NUM_TESTS test cases with dimensions M=$M, N=$N, K=$K..."

# Generate the specified number of tests
for i in $(seq 1 $NUM_TESTS); do
    echo "Generating test case $i/$NUM_TESTS..."
    ./build/generateTest "$M" "$N" "$K"
    echo "--------------------------------"
done

echo "Successfully generated $NUM_TESTS test cases in ./tests/"
echo "Test directories:"
ls -la ./tests/
