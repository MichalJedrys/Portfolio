if(NOT DEFINED CLI OR NOT DEFINED EXPECTED_VERSION)
    message(FATAL_ERROR "CLI and EXPECTED_VERSION are required")
endif()
execute_process(COMMAND "${CLI}" --version RESULT_VARIABLE result
    OUTPUT_VARIABLE output ERROR_VARIABLE error TIMEOUT 10)
if(NOT result EQUAL 0 OR NOT output STREQUAL "PlayRuntime ${EXPECTED_VERSION}\n" OR NOT error STREQUAL "")
    message(FATAL_ERROR "Unexpected version response: exit=${result}, stdout=${output}, stderr=${error}")
endif()
execute_process(COMMAND "${CLI}" --help RESULT_VARIABLE result
    OUTPUT_VARIABLE output ERROR_VARIABLE error TIMEOUT 10)
if(NOT result EQUAL 0 OR NOT output MATCHES "Usage: playruntime" OR NOT output MATCHES "not implemented" OR NOT error STREQUAL "")
    message(FATAL_ERROR "Unexpected help response: exit=${result}, stdout=${output}, stderr=${error}")
endif()
execute_process(COMMAND "${CLI}" --unknown RESULT_VARIABLE result
    OUTPUT_VARIABLE output ERROR_VARIABLE error TIMEOUT 10)
if(NOT result EQUAL 2 OR NOT output STREQUAL "" OR NOT error MATCHES "Unsupported arguments")
    message(FATAL_ERROR "Unexpected invalid-argument response: exit=${result}, stdout=${output}, stderr=${error}")
endif()
