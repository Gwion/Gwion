FROM fennecdjay/base

WORKDIR /tmp

USER root
COPY ./ gwion
RUN cd gwion && make install

WORKDIR /home/$NB_USER
USER $NB_USER
