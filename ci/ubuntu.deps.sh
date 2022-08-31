#!/bin/bash -eux

source ci/common.setup.sh

$SUDO chmod -R a+rwx /opt

$SUDO apt-get update -qq
$SUDO apt-get install wget software-properties-common

wget -nv https://github.com/jcelerier/cninja/releases/download/v3.7.5/cninja-v3.7.5-Linux.tar.gz -O cninja.tgz &
echo 'deb http://apt.llvm.org/focal/ llvm-toolchain-focal-11 main' | $SUDO tee /etc/apt/sources.list.d/llvm.list
$SUDO apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 1397BC53640DB551
$SUDO apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 15CF4D18AF4F7421

$SUDO add-apt-repository --yes ppa:ubuntu-toolchain-r/test
$SUDO add-apt-repository --yes ppa:beineri/opt-qt-5.15.0-focal

$SUDO apt purge --auto-remove cmake
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | $SUDO tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
$SUDO apt-add-repository 'deb https://apt.kitware.com/ubuntu/ focal main'

$SUDO apt-get update -qq
$SUDO apt-get upgrade -qq
$SUDO apt-get install -qq --force-yes \
    g++-10 binutils libasound-dev ninja-build cmake \
    gcovr lcov \
    qt515base qt515declarative qt515svg qt515quickcontrols2 qt515websockets qt515serialport \
    libgl1-mesa-dev \
    libavcodec-dev libavdevice-dev libavutil-dev libavfilter-dev libavformat-dev libswresample-dev \
    portaudio19-dev \
    clang-11 lld-11 libc++-11-dev libc++abi-11-dev \
    libbluetooth-dev \
    libsdl2-dev libsdl2-2.0-0 libglu1-mesa-dev libglu1-mesa \
    libgles2-mesa-dev \
    libavahi-compat-libdnssd-dev libsamplerate0-dev \
    libclang-11-dev

$SUDO apt-get remove -qq clang-8 clang-9 libclang-9-dev llvm-9-dev  libclang-10-dev llvm-10-dev
wait || true

dpkg -l | grep llvm
dpkg -l | grep clang

tar xaf cninja.tgz
$SUDO cp -rf cninja /usr/bin/

source ci/common.deps.sh
