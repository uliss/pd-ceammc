#! /bin/bash
#
# Downloads and builds tcllib and tklib
# for embedded Tcl/TK framework version.
#

# stop on error
set -e

ARCH="x86_64"
DOWNLOAD=true
BUILD=true
LEAVE=false
TCLLIB_VERSION="@TCLLIB_VERSION@"
TKLIB_VERSION="@TKLIB_VERSION@"

# set deployment target to enable weak-linking for older macOS version support
CFLAGS="-mmacosx-version-min=10.6 $CFLAGS"

# Help message
#----------------------------------------------------------
help() {
echo -e "
Usage: tcltk-libs.sh [OPTIONS] TCLLIB_VERSION TKLIB_VERSION

  Downloads and builds a tcllib and tklib for macOS

Options:
  -h,--help           display this help message

  -d,--download       download source to tcl{$VERSION}/tk{$VERSION} paths,
                      do not build

  -b,--build          (re)build from tcl{$VERSION}/tk{$VERSION} source paths,
                      do not download

Arguments:

  TCLLIB_VERSION      tcllib version (default ${TCLLIB_VERSION})
  TKLIB_VERSION       tklib version (default ${TKLIB_VERSION})

Examples:

    # build tcllib v${TCLLIB_VERSION} and tklib v${TKLIB_VERSION}
    tcltk-libs.sh

    # build tcllib version 1.20.0 and tklib v0.6.0
    tcltk-libs.sh 1.20.0 0.6.0
"
}

# Parse command line arguments
#----------------------------------------------------------
while [ "$1" != "" ] ; do
    case $1 in
        -h|--help)
            help
            exit 0
            ;;
        -d|--download)
            DOWNLOAD=true
            BUILD=false
            ;;
        -b|--build)
            DOWNLOAD=false
            BUILD=true
            ;;
        -l|--leave)
            LEAVE=true
            ;;
        *)
            break ;;
    esac
    shift 1
done

TCLTK=$1
shift 1

# Go
#----------------------------------------------------------

# change to the dir of this script
cd "@CMAKE_CURRENT_SOURCE_DIR@"

tcldir=tcllib-${TCLLIB_VERSION}
tkdir=tklib-${TKLIB_VERSION}

if [[ $DOWNLOAD == true ]] ; then
    echo "==== Downloading Tcl/Tk libs"
    # download source package
    curl -LO https://core.tcl-lang.org/tcllib/uv/tcllib-${TCLLIB_VERSION}.tar.gz
    curl -LO https://core.tcl-lang.org/tklib/tarball/tklib-${TKLIB_VERSION}.tar.gz

    # unpack
    tar -xzf tcllib-${TCLLIB_VERSION}.tar.gz
    tar -xzf tklib-${TKLIB_VERSION}.tar.gz
else
    echo  "==== Using sources in $tcldir $tkdir"

    # check source directories
    if [ ! -e "$tcldir" ] ; then
        echo "$tcldir not found"
        exit 1
    fi
    if [ ! -e "$tkdir" ] ; then
        echo "$tkdir not found"
        exit 1
    fi
fi

if [[ $BUILD == false ]] ; then
    echo  "==== Downloaded sources to $tcldir $tkdir"
    exit 0
fi

echo "==== Building Tcl/Tk libs"

WISH_APP="@CMAKE_CURRENT_SOURCE_DIR@/Wish-@WISH_APP_VERSION@.app/Contents/Frameworks"
TCLLIB_DIR="@CMAKE_CURRENT_SOURCE_DIR@/tcllib-${TCLLIB_VERSION}"
cp -R "$TCLLIB_DIR/modules/base64" "${WISH_APP}"

TKLIB_DIR="@CMAKE_CURRENT_SOURCE_DIR@/tklib-${TKLIB_VERSION}"
cp -R "$TKLIB_DIR/modules/tooltip" "${WISH_APP}"

echo  "==== Finished tcllib/tklib"
