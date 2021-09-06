#!/bin/bash

apt update -y
apt purge -y openweathermapviewer
apt install -f -y  $(realpath /root/openweathermapviewer_1.0-1_amd64.deb)

/opt/openweathermapviewer/OpenweathermapViewer

