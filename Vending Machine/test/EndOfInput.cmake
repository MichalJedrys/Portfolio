foreach(input IN ITEMS "" "1\n" "1\nn\n" "6\n" "11\n" "11\nHardPassword\n"
        "11\nHardPassword\n1\n" "11\nHardPassword\n2\n" "1\nn\n0\n1\n")
    set(input_file "${CMAKE_CURRENT_BINARY_DIR}/vending-eof-input.txt")
    file(WRITE "${input_file}" "${input}")
    execute_process(COMMAND "${PROGRAM}" INPUT_FILE "${input_file}"
        OUTPUT_VARIABLE output ERROR_VARIABLE errors RESULT_VARIABLE result TIMEOUT 5)
    if(NOT "${result}" STREQUAL "0")
        message(FATAL_ERROR "EOF did not exit successfully: ${result}\n${errors}")
    endif()
endforeach()
file(REMOVE "${input_file}")
