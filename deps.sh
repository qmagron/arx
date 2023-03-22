#!/usr/bin/bash

LIB=lib


function subinstall-emp-tool() {
  cd emp-tool
  cmake . -DCMAKE_INSTALL_PREFIX=$PWD/.. -DUSE_INTERNAL_BOOST_LIBRARY=0
  make -j
  make install -j
  cd ..
  cat include/emp-tool/utils/prg.h | sed 's/_rdseed64_step/\/\/ _rdseed64_step/g' > include/emp-tool/utils/prg.h.tmp && mv include/emp-tool/utils/prg.h.tmp include/emp-tool/utils/prg.h
}

function subinstall-emp-ot() {
  cd emp-ot
  cmake . -DCMAKE_INSTALL_PREFIX=$PWD/.. -DUSE_INTERNAL_BOOST_LIBRARY=0
  make -j
  make install -j
  cd ..
}

function subinstall_TinyGarble() {
  cd TinyGarble2.0
  cmake . -DCMAKE_INSTALL_PREFIX=$PWD/.. -DUSE_INTERNAL_BOOST_LIBRARY=0
  make -j
  make install -j
  cd ..
}



function install_tinygarble() {
  mkdir -p tinygarble && cd tinygarble
  sudo apt install -y libgmp-dev
  git clone https://github.com/IntelLabs/TinyGarble2.0.git
  git clone https://github.com/IntelLabs/emp-tool.git && cd emp-tool && git checkout -b 0.2.4 && cd ..
  git clone https://github.com/IntelLabs/emp-ot.git
  chmod u+x ./TinyGarble2.0/install_scripts/install_dependencies.sh
  sudo ./TinyGarble2.0/install_scripts/install_dependencies.sh
  subinstall-emp-tool
  subinstall-emp-ot
  subinstall_TinyGarble
  cd ..
  cd ..
}


function install() {
  install_tinygarble
}


function uninstall() {
  sudo apt autopurge -y libgmp-dev
  rm -rf $LIB
}


if [[ $1 == "install" ]]; then
  echo "Install dependencies"
  mkdir -p $LIB
  cd $LIB
  install && echo "Install completed!" || echo "[ERROR]  Install failed"
  cd ..
elif [[ $1 == "uninstall" ]]; then
  echo "Uninstall dependencies..."
  uninstall && echo "Uninstall completed!" || echo "[ERROR]  Uninstall failed"
else
  echo "Usage: $0 install|uninstall"
  exit 1
fi
