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

SET(LSB_DISTRIBUTOR_ID "unknown")
SET(LSB_RELEASE "unknown")
SET(LSB_CODENAME "unknown")
SET(LSB_BIT_TYPE "unknown")
SET(LSB_ARCH_TYPE "unknown")

# ---- (mgoettin 10/17/2011) TODO: Update this to match all OS ----
SET(LSB_PROCESSOR_ARCH ${CMAKE_SYSTEM_PROCESSOR})

# ---- Get the system bit type ----
IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(LSB_BIT_TYPE 64)
ELSE()
    SET(LSB_BIT_TYPE 32)    
ENDIF() 

# ---- Get the system LSB data ----
IF(UNIX)
  
    FIND_PROGRAM(LSB_RELEASE_EXECUTABLE lsb_release)
    IF(LSB_RELEASE_EXECUTABLE)
        # ---- Get the distribution codename ----
        EXECUTE_PROCESS(COMMAND ${LSB_RELEASE_EXECUTABLE} -s -c
                        OUTPUT_VARIABLE TMP_LSB_CODENAME
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        STRING(TOLOWER ${TMP_LSB_CODENAME} LSB_CODENAME)
        # ---- Get the release name ----
        EXECUTE_PROCESS(COMMAND ${LSB_RELEASE_EXECUTABLE} -s -r
                        OUTPUT_VARIABLE TMP_LSB_RELEASE
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        STRING(TOLOWER ${TMP_LSB_RELEASE} LSB_RELEASE)
        # ---- Get the distributor id ----
        EXECUTE_PROCESS(COMMAND ${LSB_RELEASE_EXECUTABLE} -s -i
                        OUTPUT_VARIABLE TMP_LSB_DISTRIBUTOR_ID
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        STRING(TOLOWER ${TMP_LSB_DISTRIBUTOR_ID} LSB_DISTRIBUTOR_ID)
        
        MESSAGE(STATUS "LSB-Release system information::
           LSB Distributor-ID: ${LSB_DISTRIBUTOR_ID}
           LSB Release: ${LSB_RELEASE}
           LSB Codename: ${LSB_CODENAME}
           System bit type: ${LSB_BIT_TYPE} bit")
       
    ENDIF(LSB_RELEASE_EXECUTABLE) 
ENDIF(UNIX)
