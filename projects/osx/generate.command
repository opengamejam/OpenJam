pushd $(dirname "$0")

cmake projects/ -G Xcode ../../ -DCMAKE_INSTALL_PREFIX=./build -DPLATFORM:STRING=macOS

popd