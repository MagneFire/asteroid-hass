set(SOURCES
	main.cpp
	hass.cpp
	database.cpp
	entity_model.cpp
	entity.cpp
)

set(HEADERS
	hass.h
	database.h
	entity_model.h
	entity.h
)

find_package(Qt5 COMPONENTS Sql REQUIRED)

add_library(asteroid-hass ${SOURCES} ${HEADERS} resources.qrc)
set_target_properties(asteroid-hass PROPERTIES PREFIX "")

target_link_libraries(asteroid-hass
	PUBLIC AsteroidApp
	PRIVATE Qt5::Sql)

install(TARGETS asteroid-hass
	DESTINATION ${CMAKE_INSTALL_LIBDIR})
