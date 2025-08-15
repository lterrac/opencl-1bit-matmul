#!/bin/bash

# Script to run all OpenCL tests
# Usage: ./run_tests.sh

set -e  # Exit on any error

# Check if executable exists
if [ ! -f "./build/openclMatmul" ]; then
    echo "Error: openclMatmul executable not found. Please build the project first."
    exit 1
fi

# Check if tests directory exists
if [ ! -d "./tests" ]; then
    echo "Error: tests directory not found. Please run generate_tests.sh first."
    exit 1
fi

# Find all test directories
TEST_DIRS=$(find ./tests -name "test_*" -type d | sort -V)
if [ -z "$TEST_DIRS" ]; then
    echo "Error: No test directories found in ./tests/"
    exit 1
fi

echo "Found $(echo "$TEST_DIRS" | wc -l) test directories"
echo "Running OpenCL tests..."

# Run tests on each directory
for test_dir in $TEST_DIRS; do
    test_name=$(basename "$test_dir")
    echo "Running test: $test_name"
    ./build/openclMatmul "./kernels/matmul.cl" "$test_dir"
    echo "Test $test_name completed successfully!"
    echo "---"
done

echo "All tests completed successfully!"
