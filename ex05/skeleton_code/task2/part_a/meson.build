# File       : meson.build
# Created    : Sat May 01 2021 10:04:34 AM (+0200)
# Author     : Fabian Wermelinger
# Description: Meson build definitions
# Copyright 2021 ETH Zurich. All Rights Reserved.

# project declaration must always be the first that you do
project('Homework5_prop', ['cuda'],
  version: '1.0.0',
  default_options: [
    'b_ndebug=if-release',
    'warning_level=3',
  ]
)

# add specific nvcc arguments
add_project_arguments(
  '-Xcompiler=-Wno-pedantic',
  language: 'cuda'
)

# these are the include directories that we need in our code
inc = include_directories('../../include')

executable('prop',
  files('device_properties.cu'),
  include_directories: inc,
  install: false,
)
