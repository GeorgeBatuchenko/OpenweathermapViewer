FROM debian:buster

COPY ./openweathermapviewer_1.0-1_amd64.deb .
RUN ls .
RUN apt -y update
RUN apt -f -y install ./openweathermapviewer_1.0-1_amd64.deb

ENTRYPOINT /opt/openweathermapviewer/OpenweathermapViewer.sh
