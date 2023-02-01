# - If included, system dependent information types like lsb_release name, architecture type
#   and some other useful information stored for global CMAKE use
#
#   !!! The content of these variables should not be used in cross compilation projects !!!
#
# Copyright (C) 2011 by Michael Götting  <mgoettin at techfak dot uni-bielefeld dot de>
#
# This file may be licensed under the terms of the
# GNU Lesser General Public License Version 3 (the ``LGPL''),
# or (at your option) any later version.
#
# Software distributed under the License is distributed
# on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
# express or implied. See the LGPL for the specific language
# governing rights and limitations.
#
# You should have received a copy of the LGPL along with this
# program. If not, go to http://www.gnu.org/licenses/lgpl.html
# or write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The development of this software was supported by:
#   CoR-Lab, Research Institute for Cognition and Robotics
#     Bielefeld University

# ---- Get the system LSB data ----
if(UNIX AND NOT APPLE)

    set(LSB_DISTRIBUTOR_ID "unknown")
    set(LSB_DESCRIPTION "unknown")
    set(LSB_RELEASE "unknown")
    set(LSB_CODENAME "unknown")
    set(LSB_BIT_TYPE "unknown")
    set(LSB_PROCESSOR_ARCH ${CMAKE_SYSTEM_PROCESSOR})

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(LSB_BIT_TYPE 64)
    else()
        set(LSB_BIT_TYPE 32)
    endif()

    find_program(LSB_RELEASE_EXECUTABLE lsb_release)
    if(LSB_RELEASE_EXECUTABLE)
        # ---- Get the distribution codename ----
        execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -s -c
                        OUTPUT_VARIABLE TMP_LSB_CODENAME
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        string(TOLOWER ${TMP_LSB_CODENAME} LSB_CODENAME)
        # ---- Get the release name ----
        execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -s -r
                        OUTPUT_VARIABLE TMP_LSB_RELEASE
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        string(TOLOWER ${TMP_LSB_RELEASE} LSB_RELEASE)
        # ---- Get the distributor id ----
        execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -s -i
                        OUTPUT_VARIABLE TMP_LSB_DISTRIBUTOR_ID
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        string(TOLOWER ${TMP_LSB_DISTRIBUTOR_ID} LSB_DISTRIBUTOR_ID)
        # ---- Get the description ----
        execute_process(COMMAND ${LSB_RELEASE_EXECUTABLE} -s -d
                        OUTPUT_VARIABLE LSB_DESCRIPTION
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        
        message(STATUS "LSB-Release system information::
           LSB Distributor-ID: ${LSB_DISTRIBUTOR_ID}
           LSB Description: ${LSB_DESCRIPTION}
           LSB Release: ${LSB_RELEASE}
           LSB Codename: ${LSB_CODENAME}
           LSB Arch: ${LSB_PROCESSOR_ARCH}
           System bit type: ${LSB_BIT_TYPE} bit")
       
    endif()
endif()
