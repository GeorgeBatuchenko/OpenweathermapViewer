# OpenWeatherMap Viewer
Viewer for Openwethermap.org rest api.

## Dependacies
-  Qt >= 5.11.3
-  CMake >= 3.12
-  build-essential
-  git-buildpackage
-  debhelper
- docker.io

## Preparing
At first you have to get the api_key of  Openwethermap.org to build system. To get api_key you need sign up in Openwethermap.org and get your personal key on account page.

To tell api_key to the build system run in project parent dir:

```
echo XXXXXXXXXXXXXXXXXXXXXXXXXXX > apikey.key
```

where XXXXXXXXXXXXXXXXXXXXXXXXXXX is your own api key.

## Building
If you have Qt installed by online/ofline installer you have to set some variables in your envirement. You need   Qt5_DIR=/path/to/your/Qt/lib/cmake, QT_DIR=/path/to/your/Qt/lib/cmake. 

```
mkdir -p build  && cd build
cmake /path/to/src 
make
ctest
```

## Packaging
If you want to build deb instalation package you should  clone repo by `gpb`

```
gpb clone <repo url>
cd OpenweathermapViewer
gpb export-orig
dpkg-buildpackage -us -uc
```

After this operations all debian packages apear in project parent directory. 

## Deploying
It is a good practice to build and deploy linux app in still-supported lts. So its posible to build, package, deploy and test application in debian:buster using **Docker** containers.

For use this feature run

```
deploy_and_test.sh
```
Note* you must clone repo by `gbp` and `export-orig` 

Note* you will need superuser permitions


At first start it will build docker images and it can take several tens of minutes. After that it shows OpenweathermapViewer app on your display. Next take of `deploy_and_test.sh` wiil open app emmidiately.

For more info use

```
deploy_and_test.sh --help
```