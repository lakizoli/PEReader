/////////////////////////////////////////////////////////////////////////
// $Id: lazy_flags.h 13136 2017-03-19 07:26:56Z vruppert $
/////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2001-2017  The Bochs Project
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
/////////////////////////////////////////////////////////////////////////

#ifndef BX_LAZY_FLAGS_DEF
#define BX_LAZY_FLAGS_DEF

#define BX_LF_SIGN_BIT  63

typedef struct {
  uint64_t result;
  uint64_t auxbits;
} CpuFlags;

// These are the lazy flags bits in oszapc.auxbits which hold lazy state
// of zero flag, adjust flag, carry flag, and overflow flag.

enum {
  LF_BIT_SD  = 0,         /* lazy Sign Flag Delta            */
  LF_BIT_AF  = 3,         /* lazy Adjust flag                */
  LF_BIT_PDB = 8,         /* lazy Parity Delta Byte (8 bits) */
  LF_BIT_CF  = 31,        /* lazy Carry Flag                 */
  LF_BIT_PO  = 30         /* lazy Partial Overflow = CF ^ OF */
};

const uint32_t LF_MASK_SD  = (0x01 << LF_BIT_SD);
const uint32_t LF_MASK_AF  = (0x01 << LF_BIT_AF);
const uint32_t LF_MASK_PDB = (0xFF << LF_BIT_PDB);
const uint32_t LF_MASK_CF  = (0x01 << LF_BIT_CF);
const uint32_t LF_MASK_PO = (0x01 << LF_BIT_PO);

#define ADD_COUT_VEC(op1, op2, result) \
  (((op1) & (op2)) | (((op1) | (op2)) & (~(result))))

#define SUB_COUT_VEC(op1, op2, result) \
  (((~(op1)) & (op2)) | (((~(op1)) ^ (op2)) & (result)))

#define GET_ADD_OVERFLOW(op1, op2, result, mask) \
  ((((op1) ^ (result)) & ((op2) ^ (result))) & (mask))

// *******************
// OSZAPC
// *******************

/* size, carries, result */
#define SET_FLAGS_OSZAPC_SIZE(cpu, size, lf_carries, lf_result) { \
  uint64_t temp = ((lf_carries) & (LF_MASK_AF)) | \
        (((lf_carries) >> (size - 2)) << LF_BIT_PO); \
  cpu.GetFlags().result = (uint64_t)(uint##size##_t)(lf_result); \
  if ((size) == 32) temp = ((lf_carries) & ~(LF_MASK_PDB | LF_MASK_SD)); \
  if ((size) == 16) temp = ((lf_carries) & (LF_MASK_AF)) | ((lf_carries) << 16); \
  if ((size) == 8)  temp = ((lf_carries) & (LF_MASK_AF)) | ((lf_carries) << 24); \
  cpu.GetFlags().auxbits = (uint64_t)(uint32_t)temp; \
}

/* carries, result */
#define SET_FLAGS_OSZAPC_8(cpu, carries, result) \
  SET_FLAGS_OSZAPC_SIZE(cpu, 8, carries, result)
#define SET_FLAGS_OSZAPC_16(cpu, carries, result) \
  SET_FLAGS_OSZAPC_SIZE(cpu, 16, carries, result)
#define SET_FLAGS_OSZAPC_32(cpu, carries, result) \
  SET_FLAGS_OSZAPC_SIZE(cpu, 32, carries, result)
#define SET_FLAGS_OSZAPC_64(cpu, carries, result) \
  SET_FLAGS_OSZAPC_SIZE(cpu, 64, carries, result)

/* result */
#define SET_FLAGS_OSZAPC_LOGIC_8(cpu, result_8) \
   SET_FLAGS_OSZAPC_8(cpu, 0, (result_8))
#define SET_FLAGS_OSZAPC_LOGIC_16(cpu, result_16) \
   SET_FLAGS_OSZAPC_16(cpu, 0, (result_16))
#define SET_FLAGS_OSZAPC_LOGIC_32(cpu, result_32) \
   SET_FLAGS_OSZAPC_32(cpu, 0, (result_32))
#define SET_FLAGS_OSZAPC_LOGIC_64(cpu, result_64) \
   SET_FLAGS_OSZAPC_64(cpu, BX_CONST64(0), (result_64))

/* op1, op2, result */
#define SET_FLAGS_OSZAPC_ADD_8(cpu, op1_8, op2_8, sum_8) \
  SET_FLAGS_OSZAPC_8(cpu, ADD_COUT_VEC((op1_8), (op2_8), (sum_8)), (sum_8))
#define SET_FLAGS_OSZAPC_ADD_16(cpu, op1_16, op2_16, sum_16) \
  SET_FLAGS_OSZAPC_16(cpu, ADD_COUT_VEC((op1_16), (op2_16), (sum_16)), (sum_16))
#define SET_FLAGS_OSZAPC_ADD_32(cpu, op1_32, op2_32, sum_32) \
  SET_FLAGS_OSZAPC_32(cpu, ADD_COUT_VEC((op1_32), (op2_32), (sum_32)), (sum_32))
#define SET_FLAGS_OSZAPC_ADD_64(cpu, op1_64, op2_64, sum_64) \
  SET_FLAGS_OSZAPC_64(cpu, ADD_COUT_VEC((op1_64), (op2_64), (sum_64)), (sum_64))

/* op1, op2, result */
#define SET_FLAGS_OSZAPC_SUB_8(cpu, op1_8, op2_8, diff_8) \
  SET_FLAGS_OSZAPC_8(cpu, SUB_COUT_VEC((op1_8), (op2_8), (diff_8)), (diff_8))
#define SET_FLAGS_OSZAPC_SUB_16(cpu, op1_16, op2_16, diff_16) \
  SET_FLAGS_OSZAPC_16(cpu, SUB_COUT_VEC((op1_16), (op2_16), (diff_16)), (diff_16))
#define SET_FLAGS_OSZAPC_SUB_32(cpu, op1_32, op2_32, diff_32) \
  SET_FLAGS_OSZAPC_32(cpu, SUB_COUT_VEC((op1_32), (op2_32), (diff_32)), (diff_32))
#define SET_FLAGS_OSZAPC_SUB_64(cpu, op1_64, op2_64, diff_64) \
  SET_FLAGS_OSZAPC_64(cpu, SUB_COUT_VEC((op1_64), (op2_64), (diff_64)), (diff_64))

// *******************
// OSZAP
// *******************

/* size, carries, result */
#define SET_FLAGS_OSZAP_SIZE(cpu, size, lf_carries, lf_result) { \
  uint64_t temp = ((lf_carries) & (LF_MASK_AF)) | \
        (((lf_carries) >> (size - 2)) << LF_BIT_PO); \
  if ((size) == 32) temp = ((lf_carries) & ~(LF_MASK_PDB | LF_MASK_SD)); \
  if ((size) == 16) temp = ((lf_carries) & (LF_MASK_AF)) | ((lf_carries) << 16); \
  if ((size) == 8)  temp = ((lf_carries) & (LF_MASK_AF)) | ((lf_carries) << 24); \
  cpu.GetFlags().result = (uint64_t)(uint##size##_t)(lf_result); \
  uint64_t delta_c = (cpu.GetFlags().auxbits ^ temp) & LF_MASK_CF; \
  delta_c ^= (delta_c >> 1); \
  cpu.GetFlags().auxbits = (uint64_t)(uint32_t)(temp ^ delta_c); \
}

/* carries, result */
#define SET_FLAGS_OSZAP_8(cpu, carries, result) \
  SET_FLAGS_OSZAP_SIZE(cpu, 8, carries, result)
#define SET_FLAGS_OSZAP_16(cpu, carries, result) \
  SET_FLAGS_OSZAP_SIZE(cpu, 16, carries, result)
#define SET_FLAGS_OSZAP_32(cpu, carries, result) \
  SET_FLAGS_OSZAP_SIZE(cpu, 32, carries, result)
#define SET_FLAGS_OSZAP_64(cpu, carries, result) \
  SET_FLAGS_OSZAP_SIZE(cpu, 64, carries, result)

/* op1, op2, result */
#define SET_FLAGS_OSZAP_ADD_8(cpu, op1_8, op2_8, sum_8) \
  SET_FLAGS_OSZAP_8(cpu, ADD_COUT_VEC((op1_8), (op2_8), (sum_8)), (sum_8))
#define SET_FLAGS_OSZAP_ADD_16(cpu, op1_16, op2_16, sum_16) \
  SET_FLAGS_OSZAP_16(cpu, ADD_COUT_VEC((op1_16), (op2_16), (sum_16)), (sum_16))
#define SET_FLAGS_OSZAP_ADD_32(cpu, op1_32, op2_32, sum_32) \
  SET_FLAGS_OSZAP_32(cpu, ADD_COUT_VEC((op1_32), (op2_32), (sum_32)), (sum_32))
#define SET_FLAGS_OSZAP_ADD_64(cpu, op1_64, op2_64, sum_64) \
  SET_FLAGS_OSZAP_64(cpu, ADD_COUT_VEC((op1_64), (op2_64), (sum_64)), (sum_64))

/* op1, op2, result */
#define SET_FLAGS_OSZAP_SUB_8(cpu, op1_8, op2_8, diff_8) \
  SET_FLAGS_OSZAP_8(cpu, SUB_COUT_VEC((op1_8), (op2_8), (diff_8)), (diff_8))
#define SET_FLAGS_OSZAP_SUB_16(cpu, op1_16, op2_16, diff_16) \
  SET_FLAGS_OSZAP_16(cpu, SUB_COUT_VEC((op1_16), (op2_16), (diff_16)), (diff_16))
#define SET_FLAGS_OSZAP_SUB_32(cpu, op1_32, op2_32, diff_32) \
  SET_FLAGS_OSZAP_32(cpu, SUB_COUT_VEC((op1_32), (op2_32), (diff_32)), (diff_32))
#define SET_FLAGS_OSZAP_SUB_64(cpu, op1_64, op2_64, diff_64) \
  SET_FLAGS_OSZAP_64(cpu, SUB_COUT_VEC((op1_64), (op2_64), (diff_64)), (diff_64))

// *******************
// OSZAxC
// *******************

/* size, carries, result */
#define SET_FLAGS_OSZAxC_LOGIC_SIZE(cpu, size, lf_result) { \
  bool saved_PF = cpu.GetParityFlag(); \
  SET_FLAGS_OSZAPC_SIZE(cpu, size, (uint##size##_t)(0), lf_result); \
  cpu.SetParityFlag(saved_PF); \
}

/* result */
#define SET_FLAGS_OSZAxC_LOGIC_32(cpu, result_32) \
   SET_FLAGS_OSZAxC_LOGIC_SIZE(cpu, 32, (result_32))
#define SET_FLAGS_OSZAxC_LOGIC_64(cpu, result_64) \
   SET_FLAGS_OSZAxC_LOGIC_SIZE(cpu, 64, (result_64))

#endif // BX_LAZY_FLAGS_DEF
