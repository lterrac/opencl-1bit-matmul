project(
  'OpenCL 1bit MatMul',
  ['cpp', 'c'],
  default_options: [
    'cpp_std=c++20',
    'buildtype=release',
  ],
  version: '1.0.0',
)

# Define options
printMatricesOpt = get_option('printMatrices')

####### Create OpenCL dependency

OpenCLDep = dependency('OpenCL', required: true, version: '3.0')
OpenCLHPPDep = dependency('OpenCL-CLHPP', required: true, version: '3.0')

OpenCLCppArgs = ['-DCL_HPP_TARGET_OPENCL_VERSION=300', '-DCL_TARGET_OPENCL_VERSION=300']

# Add print matrices macro
if printMatricesOpt
  OpenCLCppArgs += '-DPRINT_MATRICES'
endif

buildDep = declare_dependency(dependencies: [OpenCLDep, OpenCLHPPDep], compile_args: OpenCLCppArgs)

# Original executable
openclMatmul = executable('openclMatmul', [ 'openclMatmul.cpp'], include_directories: [ 'include' ],  dependencies: [ buildDep ])

# Test set generator
testGenerator = executable('generateTest', [ 'generateTest.cpp'], include_directories: [ 'include' ],  dependencies: [ buildDep ])

# Tests
# Find all test directories and create test cases
test_dirs = []
find_result = run_command('find', 'tests', '-name', 'test_*', '-type', 'd', check: false)
if find_result.returncode() == 0
  test_dirs = find_result.stdout().strip().split('\n')
endif

# Create test cases for each test directory
foreach test_dir : test_dirs
  if test_dir != ''
    test_name = 'opencl-test-' + test_dir.split('/')[-1]
    test(test_name, openclMatmul, args : [ meson.current_source_dir() + '/kernels/matmul.cl', meson.current_source_dir() + '/' + test_dir ])
  endif
endforeach
