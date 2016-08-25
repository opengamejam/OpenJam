pushd $(dirname "$0")

cmake . -G "Unix Makefiles" ../../  -DCMAKE_INSTALL_PREFIX=./build -DPLATFORM:STRING=KOS -DCMAKE_C_COMPILER=/opt/toolchains/dc/gcc-5.2.0/sh-elf/bin/sh-elf-gcc -DCMAKE_CXX_COMPILER=/opt/toolchains/dc/gcc-5.2.0/sh-elf/bin/sh-elf-g++ -DCMAKE_AR=/opt/toolchains/dc/gcc-5.2.0/sh-elf/bin/sh-elf-ar -DCMAKE_RANLIB=/opt/toolchains/dc/gcc-5.2.0/sh-elf/bin/sh-elf-ranlib -DCMAKE_LINKER=/opt/toolchains/dc/gcc-5.2.0/sh-elf/bin/sh-elf-ld -DKOS_BASE_DIR=/opt/toolchains/dc/kos -DKOS_PORTS_BASE_DIR=/opt/toolchains/dc/kos-ports

popd