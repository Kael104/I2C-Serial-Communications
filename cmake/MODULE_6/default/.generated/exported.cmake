set(DEPENDENT_MP_BIN2HEXMODULE_6_default_Ug5gtHsq "c:/Program Files/Microchip/xc32/v6.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFMODULE_6_default_Ug5gtHsq ${CMAKE_CURRENT_LIST_DIR}/../../../../out/MODULE_6/default.elf)
set(DEPENDENT_TARGET_DIRMODULE_6_default_Ug5gtHsq ${CMAKE_CURRENT_LIST_DIR}/../../../../out/MODULE_6)
set(DEPENDENT_BYPRODUCTSMODULE_6_default_Ug5gtHsq ${DEPENDENT_TARGET_DIRMODULE_6_default_Ug5gtHsq}/${sourceFileNameMODULE_6_default_Ug5gtHsq}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRMODULE_6_default_Ug5gtHsq}/${sourceFileNameMODULE_6_default_Ug5gtHsq}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXMODULE_6_default_Ug5gtHsq} --image ${DEPENDENT_DEPENDENT_TARGET_ELFMODULE_6_default_Ug5gtHsq} --image-generated-c ${sourceFileNameMODULE_6_default_Ug5gtHsq}.c --image-generated-h ${sourceFileNameMODULE_6_default_Ug5gtHsq}.h --image-copy-mode ${modeMODULE_6_default_Ug5gtHsq} --image-offset ${addressMODULE_6_default_Ug5gtHsq} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRMODULE_6_default_Ug5gtHsq}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFMODULE_6_default_Ug5gtHsq})
add_custom_target(
    dependent_produced_source_artifactMODULE_6_default_Ug5gtHsq 
    DEPENDS ${DEPENDENT_TARGET_DIRMODULE_6_default_Ug5gtHsq}/${sourceFileNameMODULE_6_default_Ug5gtHsq}.c
    )
