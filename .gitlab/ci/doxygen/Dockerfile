# SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
# SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
# SPDX-License-Identifier: BSD-3-Clause-Open-MPI


ARG OS_VERSION=24.04

FROM ubuntu:$OS_VERSION AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
  g++ \
  python3 \
  make \
  cmake \
  flex \
  bison \
  git ca-certificates \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /
ARG DOXYGEN_VERSION
RUN git clone --depth 1 --branch ${DOXYGEN_VERSION} https://github.com/doxygen/doxygen.git doxygen \
  && cd doxygen \
  && mkdir build \
  && cd build \
  && cmake -G "Unix Makefiles" .. \
  && make -j $(nproc) \
  && make install

FROM ubuntu:$OS_VERSION

RUN TZ=Europe/Madrid && ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y --no-install-recommends \
  graphviz default-jdk python3-pip fontconfig curl \
  rsync gcc g++ make cmake pkg-config libmbedtls-dev libyaml-cpp-dev libsctp-dev \
  && apt-get autoremove && apt-get clean && rm -rf /var/lib/apt/lists/*

ARG PLANTUML_VERSION
ENV PLANTUML_INSTALL_DIR=/opt/plantuml

COPY --from=builder /doxygen/build/bin/doxygen /usr/local/bin/
RUN mkdir -p ${PLANTUML_INSTALL_DIR} \
  && curl -L https://sourceforge.net/projects/plantuml/files/plantuml.${PLANTUML_VERSION}.jar/download -o ${PLANTUML_INSTALL_DIR}/plantuml.jar \
  && chmod +x ${PLANTUML_INSTALL_DIR}/plantuml.jar

WORKDIR /workdir
