#!/bin/bash

mkdir -p /var/lock/OpenweathermapViwer

PROJECT_FULL_PATH=$(realpath .)

xhost +si:localuser:root

if test -f /var/lock/OpenweathermapViwer/owapideploy.lock ; then
	case $1 in
		--upgrade-package)\
			docker run --rm -t -i -v $PROJECT_FULL_PATH:/root/OpenweathermapViewer owapi-deploy &&\
			docker run --rm -t -i -v $PROJECT_FULL_PATH:/root/OpenweathermapViewer -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY --entrypoint /root/OpenweathermapViewer/deploy/update-package.sh owapi-test && exit 0;;
		--build-package)\
			docker run --rm -t -i -v $PROJECT_FULL_PATH:/root/OpenweathermapViewer owapi-deploy && exit 0;;
		--reinstall-package)\
			docker run --rm -t -i -v $PROJECT_FULL_PATH:/root/OpenweathermapViewer -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY --entrypoint /root/OpenweathermapViewer/deploy/reinstall-package.sh owapi-test owapi-test && exit 0;;
		-h | --help)\
			echo Usage: &&\
			echo deploy_and_test [option] &&\
			echo --upgrade-package - rebuild packege and update test container with removing all dependancies of old version and installing new  &&\
			echo --build-package - only rebuild package &&\
			echo --reinstall-package - reinstall deb package without apgrade dependancies &&\
			echo No options - simply start test container  && exit 0;;
		*) docker run --rm -t -i -v $PROJECT_FULL_PATH:/root/OpenweathermapViewer -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY owapi-test;;
	esac
	exit 0
fi

docker image rm owapi-deploy
docker image rm owapi-test
docker build -t owapi-deploy -f deploy/deploy.dockerfile . &&\
docker run --rm -t -i -v $PROJECT_FULL_PATH:/root/OpenweathermapViewer owapi-deploy &&\
docker build -t owapi-test -f deploy/deploytest.dockerfile . &&\
echo 1 > /var/lock/OpenweathermapViwer/owapideploy.lock &&\
docker run --rm -t -i -v $PROJECT_FULL_PATH:/root/OpenweathermapViewer -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY owapi-test

