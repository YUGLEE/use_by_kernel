#！bin/sh

script_dir="$(
  cd "$(dirname "$0")" || exit
  pwd
)"

KERNEL_HOME="${script_dir}/../linux"

ROOTFS="${script_dir}/rootfs.img"
ROOTFS_MOUNT="${script_dir}/rootfs"

install_required() {
  sudo apt install bc bison flex gcc libelf-dev libssl-dev make qemu-system-x86 -y
  git pull
}

build_kernel() {
  if [ "$(ls -A "${KERNEL_HOME}")" = "" ]; then
    cd ..
    git clone -b finalwork https://github.com/L1nf/linux.git
  fi
  cd "${KERNEL_HOME}" || exit
  if [ ! -e ".config" ]; then
    make defconfig
  fi
  make -j"$(grep -c "processor" </proc/cpuinfo)"
}

build_exe() {
  cd "${script_dir}" || exit
  if [ ! -e "client" ] && [ ! -e "service" ]; then
    make
  elif [ ! -e "client" ]; then
    make client
  else
    [ ! -e "service" ]
    make service
  fi
}

move_exe() {
  cd "${script_dir}" || exit
  if [ ! -d "rootfs" ]; then
    mkdir rootfs
  fi
  if [ "$(ls -A "${ROOTFS_MOUNT}")" != "" ]; then
    sudo umount "${ROOTFS_MOUNT}"
  fi
  sudo mount "${ROOTFS}" "${ROOTFS_MOUNT}"
  mv "${script_dir}"/client "${ROOTFS_MOUNT}"
  mv "${script_dir}"/service "${ROOTFS_MOUNT}"
  sudo umount "${ROOTFS_MOUNT}"
}

run() {
  qemu-system-x86_64 -kernel "${KERNEL_HOME}"/arch/x86/boot/bzImage -hda "${ROOTFS}" -append "console=ttyS0 root=/dev/sda" -nographic -s
}

install_required
build_kernel
build_exe
move_exe
run
