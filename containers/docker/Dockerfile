# bubble_chamber_sim
#
# A container for the bubble chamber injector experiment 
#
#
FROM whit2333/geant4_base:latest
LABEL maintainer "Whitney Armstrong <warmstrong@anl.gov>"

RUN apt-get install -y libassimp-dev

RUN cd /tmp  \
    && git clone https://eicweb.phy.anl.gov/whit/bubble_chamber_sim.git \
    && cd bubble_chamber_sim && mkdir -p build \
    && cd build \
    && cmake ../. \
    && make -j20 \
    && make install

SHELL ["/bin/bash", "-c"]


###RUN cd /usr/local/lib \
###      && wget https://hallcweb.jlab.org/experiments/sane/whit/cernlib_2006.tar.gz -O - | tar -xvz 
##
##
##RUN useradd -ms /bin/bash -d /opt/saneuser saneuser \
##      && apt-get install  -y cernlib-* 
##
##ADD  saneuser_home_dir.tar /root/.
##ADD  saneuser_home_dir.tar /opt/saneuser/.
##
##RUN mkdir -p /opt/sane08 \
##      && cd /opt/sane08 \
##      && ls -lrth \
##      && git clone --depth=1 https://eicweb.phy.anl.gov/whit/Analyzer_new \
##      && git clone --depth=1 https://eicweb.phy.anl.gov/whit/sane_replay \ 
##      && cd /opt/sane08/Analyzer_new && make
##
##RUN mkdir -p /opt/saneuser/bin \
##      && cd /opt/saneuser/bin/ \
##      && wget https://hallcweb.jlab.org/experiments/sane/whit/sane08_farm_scripts.tar.gz -O - |  tar -xzv \
##      && chown -R saneuser:saneuser /opt/saneuser \
##      && chown -R saneuser:saneuser /opt/sane08 \
##      && cd /opt/sane08  \
##      && ln -s /opt/sane08/insane InSANE \
##      && cd /opt/saneuser \
##      && ln -s /opt/sane08  \
##      && ls -lrth
##
##
##USER saneuser
##WORKDIR /opt/saneuser

