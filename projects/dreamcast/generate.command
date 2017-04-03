pushd $(dirname "$0")

export KOS_WRAPPERS_PATH=${KOS_BASE}/utils/gnu_wrappers

cmake . -G "Unix Makefiles" ../../ \
    -DCMAKE_INSTALL_PREFIX=./build \
    -DPLATFORM:STRING=KOS \
    -DCMAKE_C_COMPILER=${KOS_WRAPPERS_PATH}/kos-cc \
    -DCMAKE_CXX_COMPILER=${KOS_WRAPPERS_PATH}/kos-c++ \
    -DCMAKE_AR=${KOS_WRAPPERS_PATH}/kos-ar \
    -DCMAKE_RANLIB=${KOS_WRAPPERS_PATH}/kos-ranlib \
    -DCMAKE_LINKER=${KOS_WRAPPERS_PATH}/kos-ld \
    -DCMAKE_OBJCOPY=${KOS_WRAPPERS_PATH}/kos-objcopy \
    -DCMAKE_C_FLAGS="${KOS_CFLAGS} ${KOS_LDFLAGS}" \
    -DCMAKE_CXX_FLAGS="${KOS_CFLAGS} ${KOS_CPPFLAGS} ${KOS_LDFLAGS}" \
    -DKOS_BASE_DIR=${KOS_BASE} 

popd