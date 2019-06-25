FROM debian:stretch

RUN apt-get update && \
    apt-get install -y make cmake

ADD . /usr/sysaster

WORKDIR /usr/sysaster

RUN cmake -Bbuild -Ssources && \
    cd build && \
    cmake --build .
