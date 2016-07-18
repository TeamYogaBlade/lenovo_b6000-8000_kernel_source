#!/bin/sh

option="${1}" 
case ${option} in 
   ndk) DIR="${2}"
      if [ ! -z "$DIR" -a "$DIR" != " " ]; then
         git clone https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6 "$DIR"
      fi
      if [ ! -z "$ANDROIDNDK" -a "$ANDROIDNDK" != " " ]; then
         git clone https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6 "$ANDROIDNDK"
      else
         echo 'No path for ndk download found'
      fi
      ;; 
   symlinks) DIR="${2}" 
      cd kernel/mediatek
      ln -s ../../mediatek/custom custom
      ln -s ../../mediatek/kernel kernel
      ln -s ../../mediatek/platform platform
      cd platform/mt6589/kernel/drivers/dual_ccci
      ln -s ../masp/asf asf
      cd ../../../../../../../
      ;; 
   build)  
      devicetarget="${2}"
      ndkpath="${3}"

      if [ ! -z "$ndkpath" -a "$ndkpath" != " " ]; then
         ANDROIDNDK=$ndkpath
      fi

      if [ ! -z "$ANDROIDNDK" -a "$ANDROIDNDK" != " " ]; then
         case $devicetarget in
            b6000)
              export TARGET_PRODUCT=lenovo89_tb_x8_jb2
              ;;
            8)
              export TARGET_PRODUCT=lenovo89_tb_x8_jb2
              ;;
            b8000)
              export TARGET_PRODUCT=lenovo89_tb_x10_jb2  
              ;;
            10)
              export TARGET_PRODUCT=lenovo89_tb_x10_jb2  
              ;;
            *)
              echo 'Invalid target given'
              exit 1
         esac
         export ANDROIDNDKVER=r9b
         export ANDROIDAPI=17
         export PATH=$ANDROIDNDK/bin:$PATH
         cd kernel
         export MTK_ROOT_CUSTOM=../mediatek/custom/
         make
         export MTK_ROOT_BUILD=../mediatek/build/
         ../mediatek/build/kernel/build-kernel.sh
      else
         echo 'No path for ndk download found'
      fi
      ;; 
   *)  
      echo '-------HELP-------'
      echo 'This script can help download a working version of the ndk from Googles git repository.'
      echo 'It can also fix any missing system links. Which fixes one type of common error in compilation'
      echo 'It can also build the kernel using the ndk downloaded via this script'
      echo '-------USAGE-------'
      echo 'source setup.sh ndk [NdkPath]'
      echo '        Downloads a working version of the ndk from Googles git repository. Only needs to be ran once.'
      echo '        NdkPath is the location to store the ndk, if this is not present then the envrionment var ANDROIDNDK is used.'
      echo '        Example: source setup.sh ndk "~/ndk"'
      echo 'source setup.sh symlinks'
      echo '        Fix any missing system links. Which fixes one type of common error in compilation. Only needs to be ran once.'
      echo 'source setup.sh build Target [NdkPath]'
      echo '        Build the kernel using the ndk downloaded via this script.'   
      echo '        Target is the target device. Valid Options are: "b6000", "8", "b8000", "10"'
      echo '        NdkPath is the location to store the ndk, if this is not present then the envrionment var ANDROIDNDK is used.'
      echo '        Example: source setup.sh build b6000 "~/ndk"'
      ;; 
esac 
