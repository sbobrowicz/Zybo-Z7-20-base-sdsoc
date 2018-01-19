DESCRIPTION = "PETALINUX image definition for Xilinx boards"
LICENSE = "MIT"

require recipes-core/images/petalinux-image-common.inc 

inherit extrausers
IMAGE_LINGUAS = " "

IMAGE_INSTALL = "\
		kernel-modules \
		modutils-initscripts \
		mtd-utils \
		canutils \
		lrzsz \
		openssh-sftp-server \
		alsa-utils \
		alsa-utils-aconnect \
		alsa-utils-alsaloop \
		alsa-utils-aseqdump \
		alsa-utils-aplay \
		alsa-utils-iecset \
		alsa-utils-alsaucm \
		alsa-utils-alsamixer \
		alsa-utils-amixer \
		alsa-utils-speakertest \
		alsa-utils-alsactl \
		alsa-utils-midi \
		bash-completion \
		file \
		grep \
		pciutils \
		strace \
		run-postinsts \
		alsa-lib \
		alsa-server \
		libasound \
		alsa-conf-base \
		alsa-conf \
		libsndfile1 \
		libsndfile1-bin \
		pulseaudio \
		pulseaudio-misc \
		libpulse-mainloop-glib \
		libpulsecommon \
		pulseaudio-module-console-kit \
		pulseaudio-bash-completion \
		libpulse-simple \
		libpulsecore \
		libpulse \
		pulseaudio-server \
		taglib \
		ncurses \
		alsa-state \
		alsa-states \
		libstdc++ \
		gdbserver \
		glib-2.0 \
		ldd \
		packagegroup-core-boot \
		packagegroup-core-buildessential \
		packagegroup-core-ssh-dropbear \
		packagegroup-petalinux-opencv \
		tcf-agent \
		valgrind \
		alsa-plugins \
		alsa-plugins-pulseaudio-conf \
		bridge-utils \
		gpioutil \
		kmsdemo \
		pwmdemo \
		libgpio \
		libpwm \
		libuio \
		libvrc \
		"
EXTRA_USERS_PARAMS = "usermod -P root root;"
