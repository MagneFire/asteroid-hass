cmake_minimum_required(VERSION 3.6.0)

project(asteroid-hass
	VERSION 0.0.1
	DESCRIPTION "Home Assistant app for AsteroidOS")

find_package(ECM REQUIRED NO_MODULE)
find_package(AsteroidApp REQUIRED)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ASTEROID_MODULE_PATH})

include(FeatureSummary)
include(GNUInstallDirs)
include(ECMFindQmlModule)
include(AsteroidCMakeSettings)
include(AsteroidTranslations)

ecm_find_qmlmodule(Nemo.KeepAlive 1.1)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/asteroid-hass.in
	${CMAKE_BINARY_DIR}/asteroid-hass
	@ONLY)

install(PROGRAMS ${CMAKE_BINARY_DIR}/asteroid-hass
	DESTINATION ${CMAKE_INSTALL_BINDIR})

build_translations(i18n)
add_subdirectory(src)

generate_desktop(${CMAKE_SOURCE_DIR} asteroid-hass)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
