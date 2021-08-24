#!/bin/bash

cp -R $1/libicudata.so* $4
cp -R $1/libicui18n.so* $4
cp -R $1/libicuuc.so* $4
cp -R $1/libQt5Core.so*  $4
cp -R $1/libQt5DBus.so* $4
cp -R $1/libQt5Gui.so* $4
cp -R $1/libQt5Qml.so* $4
cp -R $1/libQt5Network.so* $4
cp -R $1/libQt5QuickControls2.so* $4
cp -R $1/libQt5Quick.so* $4
cp -R $1/libQt5QuickTemplates2.so* $4
cp -R $1/libQt5QuickWidgets.so* $4
cp -R $1/libQt5Svg.so* $4
cp -R $1/libQt5Widgets.so* $4
cp -R $1/libQt5XcbQpa.so* $4

cp -R $2/* $4
cp -R $3/* $4

