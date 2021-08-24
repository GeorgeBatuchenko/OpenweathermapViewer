FROM debian:buster

RUN apt update -y
RUN apt install -y build-essential cmake 
RUN apt install -y qt5-default qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-controls2 qttools5-dev

ENTRYPOINT /root/OpenweathermapViewer/deploy/deploy.sh
