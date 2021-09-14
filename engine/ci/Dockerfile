FROM ubuntu:20.04

ADD sources.list /etc/apt/

RUN apt update -y

ENV TZ=Asia/Shanghai

# Set time zone
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo '$TZ' > /etc/timezone

RUN apt-get install wget \
       numactl build-essential uuid-dev pkg-config \
       libndctl-dev libdaxctl-dev autoconf cmake \
       python curl libz-dev doxygen pandoc -y

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y

