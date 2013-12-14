# Lenovo Yoga 8” (b6000) and 10” (b8000) Tablets Open Source Code

Downloaded from:
* http://mobilesupport.lenovo.com/en-us/products/yoga_tablet_10?tabname=downloads  
  -or-  
  http://mobilesupport.lenovo.com/en-us/products/yoga_tablet_8?tabname=downloads
  1. Open Source Code (part 1) - Yoga Tablet 8 / 10  
     http://download.lenovo.com/consumer/open_source_code/b6000-8000_source_part1.zip
    * mediatek
      * build
      * config
      * custom
      * kernel - "intended for Mediatek solution kernel specific non-customization codes"
      * platform
  2. Open Source Code (part 2) - Yoga Tablet 8 / 10  
     http://download.lenovo.com/consumer/open_source_code/b6000-8000_source_part2.zip
    * kernel
  3. Open Source Code (part 3) - Yoga Tablet 8 / 10  
     http://download.lenovo.com/consumer/open_source_code/b6000-8000_source_part3.zip
    * bionic
    * bootable
    * external

## Unpacking Downloaded ZIPs
<pre>
cd lenovo_b6000-8000_source
unzip ../b6000-8000_source_part1.zip -d .
unzip ../b6000-8000_source_part2.zip -d .
unzip ../b6000-8000_source_part3.zip -d .
</pre>

## Notes
* What is weird is that the b8000 has a MT8125 CPU, but the source code builds to a MT6589.  
  I am guessing they are basically the same CPU?

## Building
(work in progress...)

1. Install Android SDK & NDK
1. Install Android API 17 (4.2.2/CM10.1)
1. cd kernel
1. TBD...Hack a workaround to some stupid bug in the Makefile's that I am too dumb to figure out at the moment:
<pre>
ln -s ../../mediatek/platform mediatek/platform
ln -s ../../mediatek/kernel mediatek/kernel
ln -s ../../mediatek/custom mediatek/custom
</pre>
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
1. ../mediatek/build/build-kernel.sh


...  

export PATH=~/Your_Toolchain_PATH/  
for example /alps/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin  
Build Command:  
(uboot had been phase out from jb)  
### kernel
1. cd kernel
2. For b6000: export TARGET_PRODUCT=lenovo89_tb_x8_jb2
4. For b8000: export TARGET_PRODUCT=lenovo89_tb_x10_jb2
3. export MTK_ROOT_CUSTOM=../mediatek/custom/
4. make
