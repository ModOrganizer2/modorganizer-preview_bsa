cmake_minimum_required(VERSION 3.16)

add_library(preview_bsa SHARED)
mo2_configure_plugin(preview_bsa WARNINGS OFF PRIVATE_DEPENDS libbsarch)
mo2_install_target(preview_bsa)
