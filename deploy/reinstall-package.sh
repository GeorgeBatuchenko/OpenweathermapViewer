#!/bin/bash

dpkg -P openweathermapviewer

dpkg -i /root/OpenweathermapViewer/deploy/build/deb/OpenweathermapViewer-1.0-Linux.deb

/opt/OpenweathermapViewer/OpenweathermapViewer.sh

