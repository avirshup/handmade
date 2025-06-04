#!/usr/bin/env bash
# the manual commands to run, if your IDE isnae daein it 4ye

function echo-and-run() {
  echo "> $@"
  "$@"
}

set -euo pipefail
THISDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$THISDIR/cmake-build-debug"

# prompt to remove prior build dir if it already exists
if test -d "$BUILD_DIR"; then
    read -p "Remove $BUILD_DIR (y/n)? " yn
    if test "$yn" = "y"; then
      echo-and-run rm -rf "$BUILD_DIR"
      echo "removed."
      echo
    else
      echo "ABORTED: previous build directory must be removed before continuing"
      exit 1
    fi
fi

# generate the build (VCPKG required) = "reload cmake project" in CLion
# Doesn't need to run unless build conf has changed.
echo "*** Generating build ..."
echo-and-run cmake \
  -DCMAKE_BUILD_TYPE=Debug \
   -S "$THISDIR" \
   -B "$BUILD_DIR" \
   # -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/vcpkg/scripts/buildsystems/vcpkg.cmake

# FIXME: when VCPKG is enabled, cmake can't find SDL2_ttf ¯\_(ツ)_/¯
#   until this is fixed, `vcpkg.json` is renamed to `vcpkg.json.hide`
#   so IDEs won't try to automatically use it




# build the build
echo
echo "*** Building ..."
echo-and-run cmake --build \
  "$BUILD_DIR" \
  --target handmade \
  -- -j 6