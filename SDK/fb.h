#pragma once

#if !defined(BFVE_GAME_BF3) && !defined(BFVE_GAME_BF4)
  #if defined(_WIN64)
    #define BFVE_GAME_BF4 1
  #else
    #define BFVE_GAME_BF3 1
  #endif
#endif

#if defined(BFVE_GAME_BF3) && defined(BFVE_GAME_BF4)
  #error "Both BFVE_GAME_BF3 and BFVE_GAME_BF4 defined - pick one per build config."
#endif

#if defined(BFVE_GAME_BF3)
  #include "sdk.h"
#elif defined(BFVE_GAME_BF4)
  #include "sdkfb3.h"
#else
  #error "No BFVE_GAME_* macro set."
#endif
