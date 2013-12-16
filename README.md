# Lenovo Yoga 8” (b6000) and 10” (b8000) Tablets Kernel Open Source Code

Downloaded from:
* http://mobilesupport.lenovo.com/en-us/products/yoga_tablet_10?tabname=downloads  
  -or-  
  http://mobilesupport.lenovo.com/en-us/products/yoga_tablet_8?tabname=downloads
  1. Open Source Code (part 1) - Yoga Tablet 8 / 10  
     http://download.lenovo.com/consumer/open_source_code/b6000-8000_source_part1.zip  
     200MB
    * mediatek
      * build
      * config
      * custom
      * kernel - "intended for Mediatek solution kernel specific non-customization codes"
      * platform
  2. Open Source Code (part 2) - Yoga Tablet 8 / 10  
     http://download.lenovo.com/consumer/open_source_code/b6000-8000_source_part2.zip  
     121MB
    * kernel
  3. Open Source Code (part 3) - Yoga Tablet 8 / 10  
     http://download.lenovo.com/consumer/open_source_code/b6000-8000_source_part3.zip  
     103MB
    * bionic
    * bootable
    * external

## Notes
* What is weird is that the b6000/b8000 MT8125 CPU builds MT6589 code.  
  I am guessing they are basically the same CPU?

## Preparing ZIPs to Build
1. Unzip ZIPs
<pre>
unzip b6000-8000_source_part1.zip -d lenovo_b6000-8000_kernel_source
unzip b6000-8000_source_part2.zip -d lenovo_b6000-8000_kernel_source
unzip b6000-8000_source_part3.zip -d lenovo_b6000-8000_kernel_source
</pre>
1. Fix paths
<pre>
cd lenovo_b6000-8000_kernel_source/kernel/mediatek
ln -s ../../mediatek/kernel kernel
ln -s ../../mediatek/platform platform
</pre>
1. Fix scripts
<pre>
cd lenovo_b6000-8000_kernel_source/kernel
chmod +x ../mediatek/build/shell.sh
chmod +x ../mediatek/build/kernel/build-kernel.sh
</pre>
   Edit build-kernel.sh per ...

## Build
1. Install Android SDK & NDK
1. Install Android API 17 (4.2.2/CM10.1)
1. Set up environment:
<pre>
export ANDROIDSDK=$HOME/android/sdk
export ANDROIDNDK=$HOME/android/ndk
export ANDROIDNDKVER=r9b
export ANDROIDAPI=17
export PATH=$PATH:$ANDROIDNDK/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86_64/bin
export MTK_ROOT_CUSTOM=../mediatek/custom/
</pre>
   For b6000:
<pre>
export TARGET_PRODUCT=lenovo89_tb_x8_jb2
</pre>
   For b8000:
<pre>
export TARGET_PRODUCT=lenovo89_tb_x10_jb2
</pre>
<pre>
cd lenovo_b6000-8000_kernel_source/kernel
make
</pre>

## Image
First, follow the above Build steps; then...  
<pre>
cd lenovo_b6000-8000_kernel_source/kernel
export MTK_ROOT_BUILD=../mediatek/build/
../mediatek/build/kernel/build-kernel.sh
</pre>
