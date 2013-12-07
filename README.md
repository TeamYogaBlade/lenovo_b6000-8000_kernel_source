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

## Processing

<pre>
cd lenovo_b6000-8000_source
unzip ../b6000-8000_source_part1.zip -d .
unzip ../b6000-8000_source_part2.zip -d .
unzip ../b6000-8000_source_part3.zip -d .
</pre>

## Building
export PATH=~/Your_Toolchain_PATH/
for example /alps/prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.6/bin
Build Command:
(uboot had been phase out from jb)
kernel
======
1. cd kernel
2. For b6000: export TARGET_PRODUCT=lenovo89_tb_x8_jb2
4. For b8000: export TARGET_PRODUCT=lenovo89_tb_x10_jb2
3. export MTK_ROOT_CUSTOM=../mediatek/custom/
4. make
