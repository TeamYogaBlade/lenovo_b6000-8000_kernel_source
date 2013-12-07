#!/bin/bash
##############################################################
# Program:
#	Program creates YuSu u-boot and logo image 
#
# History:
# 2010/11/16    Kirby:    re-factorize
# 2010/04/03	Shu-Hsin: add u-boot header
# 2010/01/19	Shu-Hsin: add OPPO phone support
# 2010/01/18	Shu-Hsin: centralize boot storage and chip definition
#

##############################################################
# 0. help                                                    #
#    make procedure:                                         #
#     1. make <project>_config                               #
#     2. make                                                #
#    generate image:                                         #
#     uboot-<project>.bin                                    #
#     logo.bin                                               #
# /note/ please DO NOT use project / chip name in this file  #
##############################################################

makejobs=${MAKEJOBS}

##############################################################
# 1. parse options                                           #
##############################################################

CUR_DIR=`pwd`

usage() {
    echo "Usage: build.sh [-c] [Project Name]"
    echo "   -c: clean before building"
    echo "   when omitting project name, env variable "TARGET_PRODUCT" will be checked and used."
}


##############################################################
# Get options flag

while getopts "ch?" option
do
    case $option in
      c) CLEAN="yes";;
      h) usage; exit 0;;
      [?]) usage; exit 0;;
    esac
done

shift $(($OPTIND - 1))

if [ "$HELP" == "yes" ]; then
    usage; exit 1;
fi

if [ "${PROJECT}" != "" ]; then export TARGET_PRODUCT=${PROJECT}; fi
if [ "$1" != "" ]; then export TARGET_PRODUCT=$1; fi

##############################################################
# 2. initialize variables                                    #
##############################################################
source ../../../mediatek/build/shell.sh ../../../ uboot

UBOOT_IMAGE=${CUR_DIR}/uboot_${MTK_PROJECT}.bin
UBOOT_LOGO_PATH="${CUR_DIR}/${MTK_PATH_CUSTOM}/logo"
MKIMG="${CUR_DIR}/${MTK_ROOT_BUILD}/tools/mkimage"

##############################################################
# 3. building                                                #
##############################################################
if [ ! -x mkconfig ]; then chmod a+x mkconfig; fi
if [ "$CLEAN" == "yes" ]; then make distclean; fi
if [ -f "${CUR_DIR}/u-boot.bin" ]; then rm "${CUR_DIR}/u-boot.bin"; fi

make ${MTK_PROJECT}_config; make ${makejobs}

if [ ! -f "${CUR_DIR}/u-boot.bin" ]; then
    echo "COMPILE FAIL !!!!!!!!!!!!!!!!"
    exit 1
fi

if [ ! -x ${MKIMG} ]; then chmod a+x ${MKIMG}; fi

if [ -d ${UBOOT_LOGO_PATH} ]; then
    cd  ${UBOOT_LOGO_PATH}; ./update ${BOOT_LOGO}; cd - > /dev/null
    ${MKIMG} ${UBOOT_LOGO_PATH}/${BOOT_LOGO}.raw LOGO > ${CUR_DIR}/logo.bin
fi

${MKIMG} ${CUR_DIR}/u-boot.bin UBOOT > ${UBOOT_IMAGE}


##############################################################
# 4. code size statistics                                    #
##############################################################
echo "===================== building warning ========================"
echo "the following guy's code size is a bit large, ( > 2500 bytes )"
echo "please note it and reduce it if possible !"
echo "for more detail, please refer to <u-boot-code-size-report.txt>"
echo "---------------------------------------------------------------"
echo ${MTK_PROJECT} image size is `stat --printf="%s bytes\n" ${UBOOT_IMAGE}`
echo "---------------------------------------------------------------"
echo "                      CODE SIZE REPORT                         "
echo "---------------------------------------------------------------"
echo "size(bytes)     file  ( size > 2500 bytes )"
echo "---------------------------------------------------------------"
size `ls ${MTK_PATH_PLATFORM}/*.o` > u-boot-code-size-report.txt
awk '$1 ~ /^[0-9]/ { if ($4>2500) print $4 "\t\t" $6}' < u-boot-code-size-report.txt | sort -rn | sed 's/\.\.\/\/\?//g'
echo "==============================================================="
echo "UBoot-2010.06 is built successfully!!"

copy_to_legacy_download_flash_folder   ${UBOOT_IMAGE} logo.bin
