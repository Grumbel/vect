# Vect - A simple Vector-Graphic editor for Retriever
# Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.0)
project(vect VERSION 0.6.0)

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
pkg_search_module(ALLEGRO REQUIRED allegro)

file(GLOB VECT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cc)

add_executable(vect ${VECT_SOURCES})
target_link_libraries(vect ${ALLEGRO_LIBRARIES})
target_include_directories(vect PUBLIC ${ALLEGRO_INCLUDE_DIRS})
target_compile_definitions(vect PUBLIC ALLEGRO_NO_FIX_ALIASES)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/graphics ${CMAKE_CURRENT_SOURCE_DIR}/default.pal
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/vect.sh.in
  ${CMAKE_BINARY_DIR}/vect.sh)

install(TARGETS vect
  RUNTIME DESTINATION ${CMAKE_INSTALL_LIBEXECDIR})

install(FILES
  ${CMAKE_BINARY_DIR}/vect.sh
  RENAME vect
  PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY graphics DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}
  PATTERN "*~" EXCLUDE)

install(FILES default.pal DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})

enable_testing()

# EOF #
