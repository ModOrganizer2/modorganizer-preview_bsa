cmake_minimum_required(VERSION 3.16)

add_library(preview_bsa SHARED)
mo2_configure_plugin(preview_bsa WARNINGS OFF PRIVATE_DEPENDS bsatk)
target_include_directories(preview_bsa PUBLIC ${DDS_ROOT})
mo2_install_target(preview_bsa)
