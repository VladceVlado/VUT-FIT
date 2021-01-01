FROM docker.elastic.co/beats/packetbeat:7.10.0

COPY packetbeat.yaml /usr/share/packetbeat/packetbeat.yml

USER root

RUN chown root:packetbeat /usr/share/packetbeat/packetbeat.yml

USER packetbeat
