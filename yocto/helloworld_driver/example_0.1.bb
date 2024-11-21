DESCRIPTION = "My Custom Driver"
LICENSE = "CLOSED"

SRC_URI = "file://mydriver.c \
           file://mydriver.h \
           file://Makefile"

S = "${WORKDIR}"

inherit module

EXTRA_OEMAKE = ""

do_configure() {
    oe_runmake -C ${STAGING_KERNEL_DIR} M=${S} clean
}

do_compile() {
    oe_runmake -C ${STAGING_KERNEL_DIR} M=${S} modules
}

do_install() {
    install -d ${D}/lib/modules/${KERNEL_VERSION}/extra
    install -m 0644 ${B}/mydriver.ko ${D}/lib/modules/${KERNEL_VERSION}/extra
}

FILES_${PN} += "/lib/modules/${KERNEL_VERSION}/extra/mydriver.ko"
