set(OUTPUT_NAME protogenBadge)
add_executable(${OUTPUT_NAME} 
        main.cpp
        srcs/Menu.cpp
        srcs/CustomBadger.cpp
        srcs/SubMenuItem.cpp
        srcs/ImageMenuItem.cpp
        srcs/ClearMenuItem.cpp
        srcs/SettingsMenu.cpp
)

target_link_libraries(${OUTPUT_NAME}
        badger2040
        hardware_spi
        hardware_uart
)

# enable usb output
pico_enable_stdio_usb(${OUTPUT_NAME} 1)

pico_add_extra_outputs(${OUTPUT_NAME})

add_compile_options(-Wall
        -Werror
        -Wpedantic
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )