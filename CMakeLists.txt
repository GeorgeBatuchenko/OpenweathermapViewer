cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME OpenweathermapViewer)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(READ apikey.key OWM_API_KEY)

enable_testing()

find_package(QT NAMES Qt5 COMPONENTS Core Quick LinguistTools Test Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools Test Network REQUIRED)

set(TS_FILES ${PROJECT_NAME}_ru_RU.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/components/currentweather/currentweathermodel.cpp
        src/components/currentweather/currentweathermodel.h
        src/components/dailyforecast/dailyforecastmodel.cpp
        src/components/dailyforecast/dailyforecastmodel.h
        src/components/hourlyforecast/hourlyforecastmodel.cpp
        src/components/hourlyforecast/hourlyforecastmodel.h
        src/pages/cities/citieslistmodel.cpp
        src/pages/cities/citieslistmodel.h
        src/modules/openweathermapapiclient/openweathermapapiclient.cpp
        src/modules/openweathermapapiclient/openweathermapapiclient.h
        src/modules/openweathermapapiclient/abstractopenweathermapapiclient.cpp
        src/modules/openweathermapapiclient/abstractopenweathermapapiclient.h
        src/modules/appsettings/appsettings.h
        src/modules/appsettings/appsettings.cpp
        src/modules/appsettings/abstractappsettings.h
        src/modules/appsettings/abstractappsettings.cpp
        qml.qrc
        ${TS_FILES}
)

set(TARGET_NAME ${PROJECT_NAME})

add_executable(${TARGET_NAME}
    ${PROJECT_SOURCES}
)

#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_compile_definitions(${TARGET_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${TARGET_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Network)

configure_file(appsettings.ini.in ${CMAKE_BINARY_DIR}/appsettings.ini @ONLY)

add_subdirectory(test)


# INSTALATION (tested on debian:)
# Instalation used for cpack DEB package generation QT packages must be installed by apt
# dependancies: qt5-default qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-controls2 qttools5-dev

#${CMAKE_BINARY_DIR}/deb

install(
	TARGETS ${TARGET_NAME}
	DESTINATION /opt/OpenweathermapViewer
)

install(
	PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/debian/OpenweathermapViewer.sh
	DESTINATION /opt/OpenweathermapViewer
)

install(
	FILES ${CMAKE_BINARY_DIR}/appsettings.ini
	DESTINATION /opt/OpenweathermapViewer
)

install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/debian/OpenweathermapViewer.desktop
	DESTINATION /usr/share/applications/OpenweathermapViewer
)

install(
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/images/openweathermapviewer.png
	DESTINATION /usr/share/OpenweathermapViewer
)

set(OWMV_QT_LIBS_PATH /usr/lib/x86_64-linux-gnu)
if(DEFINED ENV{OWMV_QT_LIBS})
	set(OWMV_QT_LIBS_PATH $ENV{OWMV_QT_LIBS})
endif()

set(OWMV_QT_QML_PATH /usr/lib/x86_64-linux-gnu/qt5/qml)
if(DEFINED ENV{OWMV_QT_QML})
	set(OWMV_QT_QML_PATH $ENV{OWMV_QT_QML})
endif()

set(OWMV_QT_PLUGIN_PATH /usr/lib/x86_64-linux-gnu/qt5/plugins)
if(DEFINED ENV{OWMV_QT_PLUGIN})
	set(OWMV_QT_PLUGIN_PATH $ENV{OWMV_QT_PLUGIN})
endif()

set(CPACK_dest_dir ${CMAKE_BINARY_DIR}/deb/_CPack_Packages/Linux/DEB/OpenweathermapViewer-1.0-Linux/opt/OpenweathermapViewer)

install (
CODE "execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/debian/qt_deps_copy.sh ${OWMV_QT_LIBS_PATH} ${OWMV_QT_PLUGIN_PATH} ${OWMV_QT_QML_PATH} ${CPACK_dest_dir} )"
)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "George Batuchenko")
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/deb)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "adwaita-icon-theme, at-spi2-core, avahi-daemon, bind9-host, dbus, dbus-user-session, dconf-gsettings-backend, dconf-service, dmsetup, fontconfig, fontconfig-config, fonts-dejavu-core, geoclue-2.0, geoip-database, glib-networking, glib-networking-common, glib-networking-services, gsettings-desktop-schemas, gstreamer1.0-plugins-base, gtk-update-icon-cache, hicolor-icon-theme, iio-sensor-proxy, iso-codes, krb5-locales, libapparmor1, libargon2-1, libatk-bridge2.0-0, libatk1.0-0, libatk1.0-data, libatspi2.0-0, libavahi-client3, libavahi-common-data, libavahi-common3, libavahi-core7, libavahi-glib1, libbind9-161, libbrotli1, libbsd0, libcairo-gobject2, libcairo2, libcdparanoia0, libclang1-7, libcolord2, libcroco3, libcryptsetup12, libcups2, libdaemon0, libdatrie1, libdbus-1-3, libdconf1, libdevmapper1.02.1, libdns1104, libdouble-conversion1, libdrm-amdgpu1, libdrm-common, libdrm-intel1, libdrm-nouveau2, libdrm-radeon1, libdrm2, libedit2, libegl-mesa0, libegl1, libepoxy0, libexpat1, libfontconfig1, libfreetype6, libfribidi0, libfstrm0, libgbm1, libgdbm-compat4, libgdbm6, libgdk-pixbuf2.0-0, libgdk-pixbuf2.0-bin, libgdk-pixbuf2.0-common, libgeoip1, libgl1, libgl1-mesa-dri, libglapi-mesa, libgles1, libgles2, libglib2.0-0, libglib2.0-data, libglu1-mesa, libglvnd0, libglx-mesa0, libglx0, libgraphite2-3, libgssapi-krb5-2, libgstreamer-plugins-base1.0-0, libgstreamer1.0-0, libgtk-3-0, libgtk-3-bin, libgtk-3-common, libharfbuzz0b, libhyphen0, libice6, libicu63, libidn11, libinput-bin, libinput10, libip4tc0, libisc1100, libisccc161, libisccfg163, libjbig0, libjim0.77, libjpeg62-turbo, libjson-c3, libjson-glib-1.0-0, libjson-glib-1.0-common, libk5crypto3, libkeyutils1, libkmod2, libkrb5-3, libkrb5support0, liblcms2-2, libllvm7, liblmdb0, liblwres161, libmbim-glib4, libmbim-proxy, libmm-glib0, libmtdev1, libnl-3-200, libnl-genl-3-200, libnl-route-3-200, libnotify4, libnss-mdns, libnss-systemd, libogg0, libopengl0, libopus0, liborc-0.4-0, libpam-systemd, libpango-1.0-0, libpangocairo-1.0-0, libpangoft2-1.0-0, libpciaccess0, libpcre2-16-0, libpcsclite1, libperl5.28, libpixman-1-0, libpng16-16, libpolkit-gobject-1-0, libprotobuf-c1, libproxy1v5, libpsl5, libqmi-glib5, libqmi-proxy, libssl1.1, libthai-data, libthai0, libtheora0, libtiff5, libusb-1.0-0, libvisual-0.4-0, libvorbis0a, libvorbisenc2, libvulkan1, libwacom-bin, libwacom-common, libwacom2, libwayland-client0, libwayland-cursor0, libwayland-egl1, libwayland-server0, libwebp6, libwoff1, libx11-6, libx11-data, libx11-xcb1, libxau6, libxcb-dri2-0, libxcb-dri3-0, libxcb-glx0, libxcb-icccm4, libxcb-image0, libxcb-keysyms1, libxcb-present0, libxcb-randr0, libxcb-render-util0, libxcb-render0, libxcb-shape0, libxcb-shm0, libxcb-sync1, libxcb-util0, libxcb-xfixes0, libxcb-xinerama0, libxcb-xkb1, libxcb1, libxcomposite1, libxcursor1, libxdamage1, libxdmcp6, libxext6, libxfixes3, libxi6, libxinerama1, libxkbcommon-x11-0, libxkbcommon0, libxml2, libxrandr2, libxrender1, libxshmfence1, libxslt1.1, libxtst6, libxxf86vm1, lsb-base, modemmanager, netbase, notification-daemon, perl, perl-modules-5.28, publicsuffix, readline-common, sensible-utils, shared-mime-info, systemd, systemd-sysv, ucf, udev, usb-modeswitch, usb-modeswitch-data, wpasupplicant, x11-common, xdg-user-dirs, xkb-data, xorg-sgml-doctools, libcurl3-gnutls, libsm6")
include (CPack)

