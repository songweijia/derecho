#!/bin/bash
set -eu
INSTALL_PREFIX="/usr/local"
if [[ $# -gt 0 ]]; then
    INSTALL_PREFIX=$1
fi

echo "Using INSTALL_PREFIX=${INSTALL_PREFIX}"

git clone https://github.com/mpmilano/mutils-tasks.git
cd mutils-tasks
# git checkout e9584168390eb3fac438a443f3bb93ed692e972a
git checkout 680d456141f4b6e29f6ede04e9e7b56763053976
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} ..
make -j `lscpu | grep "^CPU(" | awk '{print $2}'`
make install
cd ../..
echo "Cleanup by executing 'rm -rf mutils-tasks'? [Y/n]"
read c
if [[ $c == [nN] ]];
then
    echo "keep mutils-tasks"
else
    rm -rf mutils-tasks
fi

