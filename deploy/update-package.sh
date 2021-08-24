#!/bin/bash

apt apdate -y
dpkg -P openweathermapviewer
apt autoremove -y

dpkg -i /root/OpenweathermapViewer/deploy/build/deb/OpenweathermapViewer-1.0-Linux.deb
apt install -f -y
dpkg -i /root/OpenweathermapViewer/deploy/build/deb/OpenweathermapViewer-1.0-Linux.deb

/opt/OpenweathermapViewer/OpenweathermapViewer.sh

