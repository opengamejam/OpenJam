pushd $(dirname "$0")

cmake projects/ -G Xcode ../../ -DPLATFORM:STRING=IOS

popd