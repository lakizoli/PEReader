#pragma once

#include "config.h"

#if BX_USE_MEM_SMF
// if static member functions on, then there is only one memory
#  define BX_MEM_SMF  static
#  define BX_MEM_THIS BX_MEM(0)->
#else
#  define BX_MEM_SMF
#  define BX_MEM_THIS this->
#endif 

class BX_CPU_C;

class BX_MEM_C {
public:
  BX_MEM_SMF Bit64u  get_memory_len (void);
  BX_MEM_SMF Bit8u*  getHostMemAddr(BX_CPU_C *cpu, bx_phy_address addr, unsigned rw);

  // Note: accesses should always be contained within a single page
  BX_MEM_SMF void    readPhysicalPage(BX_CPU_C *cpu, bx_phy_address addr, unsigned len, void *data);
  BX_MEM_SMF void    writePhysicalPage(BX_CPU_C *cpu, bx_phy_address addr, unsigned len, void *data); 
};
