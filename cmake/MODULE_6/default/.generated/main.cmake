include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(MODULE_6_default_library_list )

set(OUTPUT_SUFFIX elf)
# Handle files with suffix s, for group default-XC32
if(MODULE_6_default_default_XC32_FILE_TYPE_assemble)
add_library(MODULE_6_default_default_XC32_assemble OBJECT ${MODULE_6_default_default_XC32_FILE_TYPE_assemble})
    MODULE_6_default_default_XC32_assemble_rule(MODULE_6_default_default_XC32_assemble)
    list(APPEND MODULE_6_default_library_list "$<TARGET_OBJECTS:MODULE_6_default_default_XC32_assemble>")

endif()

# Handle files with suffix S, for group default-XC32
if(MODULE_6_default_default_XC32_FILE_TYPE_assembleWithPreprocess)
add_library(MODULE_6_default_default_XC32_assembleWithPreprocess OBJECT ${MODULE_6_default_default_XC32_FILE_TYPE_assembleWithPreprocess})
    MODULE_6_default_default_XC32_assembleWithPreprocess_rule(MODULE_6_default_default_XC32_assembleWithPreprocess)
    list(APPEND MODULE_6_default_library_list "$<TARGET_OBJECTS:MODULE_6_default_default_XC32_assembleWithPreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(MODULE_6_default_default_XC32_FILE_TYPE_compile)
add_library(MODULE_6_default_default_XC32_compile OBJECT ${MODULE_6_default_default_XC32_FILE_TYPE_compile})
    MODULE_6_default_default_XC32_compile_rule(MODULE_6_default_default_XC32_compile)
    list(APPEND MODULE_6_default_library_list "$<TARGET_OBJECTS:MODULE_6_default_default_XC32_compile>")

endif()

# Handle files with suffix cpp, for group default-XC32
if(MODULE_6_default_default_XC32_FILE_TYPE_compile_cpp)
add_library(MODULE_6_default_default_XC32_compile_cpp OBJECT ${MODULE_6_default_default_XC32_FILE_TYPE_compile_cpp})
    MODULE_6_default_default_XC32_compile_cpp_rule(MODULE_6_default_default_XC32_compile_cpp)
    list(APPEND MODULE_6_default_library_list "$<TARGET_OBJECTS:MODULE_6_default_default_XC32_compile_cpp>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(MODULE_6_default_default_XC32_FILE_TYPE_dependentObject)
add_library(MODULE_6_default_default_XC32_dependentObject OBJECT ${MODULE_6_default_default_XC32_FILE_TYPE_dependentObject})
    MODULE_6_default_default_XC32_dependentObject_rule(MODULE_6_default_default_XC32_dependentObject)
    list(APPEND MODULE_6_default_library_list "$<TARGET_OBJECTS:MODULE_6_default_default_XC32_dependentObject>")

endif()


# Main target for this project
add_executable(MODULE_6_default_image_Ug5gtHsq ${MODULE_6_default_library_list})

set_target_properties(MODULE_6_default_image_Ug5gtHsq PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    RUNTIME_OUTPUT_DIRECTORY "${MODULE_6_default_output_dir}")
target_link_libraries(MODULE_6_default_image_Ug5gtHsq PRIVATE ${MODULE_6_default_default_XC32_FILE_TYPE_link})
# Add the link options from the rule file.
MODULE_6_default_link_rule( MODULE_6_default_image_Ug5gtHsq)

# Add bin2hex target for converting built file to a .hex file.
string(REGEX REPLACE [.]elf$ .hex MODULE_6_default_image_name_hex ${MODULE_6_default_image_name})
add_custom_target(MODULE_6_default_Bin2Hex ALL
    COMMAND ${MP_BIN2HEX} \"${MODULE_6_default_output_dir}/${MODULE_6_default_image_name}\"
    DEPENDS "${MODULE_6_default_output_dir}/${MODULE_6_default_image_name}"
    BYPRODUCTS ${MODULE_6_default_output_dir}/${MODULE_6_default_image_name_hex}
    COMMENT "Convert built file to .hex")
add_dependencies(MODULE_6_default_Bin2Hex MODULE_6_default_image_Ug5gtHsq)




