#!/bin/bash

cd /root/OpenweathermapViewer

if [ -d deploy/build ]; then 
	rm -Rf deploy/build; 
fi

mkdir deploy/build &&\
cd deploy/build &&\
cmake ../../ -DCMAKE_BUILD_TYPE=Release &&\
make &&\
ctest &&\
cpack &&\
cd .. && chmod -R +w build
 
