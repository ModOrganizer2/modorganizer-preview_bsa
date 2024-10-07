cmake_minimum_required(VERSION 3.16)

find_package(mo2-cmake CONFIG REQUIRED)
find_package(mo2-uibase CONFIG REQUIRED)

find_package(ZLIB REQUIRED)
find_package(lz4 CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS thread interprocess)
find_package(mo2-dds-header CONFIG REQUIRED)
find_package(mo2-bsatk CONFIG REQUIRED)

add_library(preview_bsa SHARED)
mo2_configure_plugin(preview_bsa WARNINGS 4)
target_link_libraries(preview_bsa PRIVATE mo2::uibase mo2::bsatk)
mo2_install_plugin(preview_bsa)
