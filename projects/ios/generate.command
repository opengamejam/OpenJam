pushd $(dirname "$0")

cmake projects/ -G Xcode ../../ \
    -DCMAKE_INSTALL_PREFIX=./build \
    -DPLATFORM:STRING=iOS \
    -DCMAKE_OSX_SYSROOT=iphoneos \
	-DCMAKE_OSX_ARCHITECTURES="armv7;arm64;i386" \
	-DCMAKE_CXX_FLAGS="-std=c++11 -fno-rtti -fno-exceptions -Wall" \
    -DCMAKE_C_COMPILER_WORKS=TRUE \
	-DCMAKE_CXX_COMPILER_WORKS=TRUE \
	-DCMAKE_CROSSCOMPILING=TRUE
popd