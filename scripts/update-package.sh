#!/bin/bash

apt update -y
apt purge -y openweathermapviewer
apt autoremove -y

apt -f -y install /root/openweathermapviewer_1.0-1_amd64.deb

/opt/openweathermapviewer/OpenweathermapViewer.sh

