//===- test.cpp -------------------------------------------------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// (c) Copyright 2021 Xilinx Inc.
//
//===----------------------------------------------------------------------===//

#include "test_library.h"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>
#include <xaiengine.h>

#include "aie_inc.cpp"

int main(int argc, char *argv[]) {
  printf("test start.\n");

  aie_libxaie_ctx_t *_xaie = mlir_aie_init_libxaie();
  mlir_aie_init_device(_xaie);

  mlir_aie_clear_tile_memory(_xaie, 1, 3);

  mlir_aie_configure_cores(_xaie);
  mlir_aie_configure_switchboxes(_xaie);
  mlir_aie_initialize_locks(_xaie);
  mlir_aie_configure_dmas(_xaie);
  mlir_aie_start_cores(_xaie);

  int errors = 0;

  printf("Waiting for the result ...\n");
  if (!mlir_aie_acquire_prime_output_lock(_xaie, 1, 100000)) {
    printf("ERROR: timeout hit!\n");
  }

  mlir_aie_check("primes[0]", mlir_aie_read_buffer_prime_output(_xaie, 0), 7,
                 errors);
  mlir_aie_check("primes[1]", mlir_aie_read_buffer_prime_output(_xaie, 1), 11,
                 errors);
  mlir_aie_check("primes[2]", mlir_aie_read_buffer_prime_output(_xaie, 2), 13,
                 errors);

  int res = 0;
  if (!errors) {
    printf("PASS!\n");
    res = 0;
  } else {
    printf("Fail!\n");
    res = -1;
  }
  mlir_aie_deinit_libxaie(_xaie);

  printf("test done.\n");
  return res;
}
