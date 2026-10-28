# Configuration file for custom user settings

# Locate the Smart Configurator (RASC) executable.
if(DEFINED RASC_EXE_PATH)
    set(_rasc_exe "${RASC_EXE_PATH}")
    message("Using RASC_EXE_PATH defined via CLI -D: ${_rasc_exe}")
elseif(DEFINED ENV{RASC_EXE_PATH})
    set(_rasc_exe "$ENV{RASC_EXE_PATH}")
    message("Using RASC_EXE_PATH defined in environment: ${_rasc_exe}")
else ()
    # No override given: auto-detect the newest installed Smart Configurator so
    # this works on any PC regardless of the exact sc_v*/fsp_v* version installed.
    file(GLOB _rasc_candidates
        "C:/Renesas/RA/sc_v*/eclipse/rasc.exe"
        "$ENV{LOCALAPPDATA}/Programs/Renesas/RA/sc_v*/eclipse/rasc.exe")
    if(_rasc_candidates)
        list(SORT _rasc_candidates)
        list(GET _rasc_candidates -1 _rasc_exe)
        message("Auto-detected RASC_EXE_PATH: ${_rasc_exe}")
    else()
        set(_rasc_exe "C:/Renesas/RA/sc_v2025-12_fsp_v6.4.0/eclipse/rasc.exe")
        message("Using RASC_EXE_PATH fallback default: ${_rasc_exe} "
                "(set RASC_EXE_PATH env var or -DRASC_EXE_PATH= to override)")
    endif()
endif()

# The real executable is always kept in RASC_GENERATE_EXE_PATH, so CMakeLists.txt
# can still generate missing FSP content (ra/, ra_cfg/, ra_gen/, ...) when
# RASC_EXE_PATH is replaced by "echo" below.
set(RASC_GENERATE_EXE_PATH "${_rasc_exe}")

if(RENESAS_IDE AND RENESAS_IDE STREQUAL "e2studio")
    set(RASC_EXE_PATH "echo")
    message(NOTICE "RENESAS_IDE is defined, so RASC_EXE_PATH set to echo for information purposes")
else()
    set(RASC_EXE_PATH "${_rasc_exe}")
endif()
