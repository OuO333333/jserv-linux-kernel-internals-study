DESCRIPTION = "My Custom Driver"
LICENSE = "CLOSED"

SRC_URI = "file://mydriver.c \
           file://mydriver.h \
           file://Makefile"

S = "${WORKDIR}"

inherit module

EXTRA_OEMAKE = ""

# 確保在 do_configure 中只調用 oe_runmake，而不需要手動傳遞 M= 參數
do_configure() {
    echo "Cleaning previous builds in the kernel source tree..."
    oe_runmake -C ${STAGING_KERNEL_DIR} M=${S} clean
}

do_compile() {
    echo "Compiling kernel module..."
    oe_runmake -C ${STAGING_KERNEL_DIR} M=${S} modules
}

do_install() {
    echo "Installing kernel module..."
    install -d ${D}/lib/modules/${KERNEL_VERSION}/extra
    install -m 0644 ${B}/mydriver.ko ${D}/lib/modules/${KERNEL_VERSION}/extra

    # 安裝自動加載配置文件
    install -d ${D}/etc/modules-load.d
    echo "mydriver" > ${D}/etc/modules-load.d/mydriver.conf
}

FILES_${PN} += "/lib/modules/${KERNEL_VERSION}/extra/mydriver.ko"
