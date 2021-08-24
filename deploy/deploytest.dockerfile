FROM debian:buster

COPY deploy/build/deb/OpenweathermapViewer-1.0-Linux.deb /root
RUN apt update -y
RUN dpkg -i /root/OpenweathermapViewer-1.0-Linux.deb; exit 0
RUN apt install -f -y
RUN dpkg -i /root/OpenweathermapViewer-1.0-Linux.deb

ENTRYPOINT /opt/OpenweathermapViewer/OpenweathermapViewer.sh
