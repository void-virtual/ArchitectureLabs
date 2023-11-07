FROM ubuntu
ENV TZ=Europe/Moscow
RUN apt-get clean && apt-get update -y 
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata git sudo build-essential autoconf libtool libssl-dev zlib1g-dev librdkafka-dev pkg-config cmake gcc git g++ mysql-client libmysqlclient-dev libboost-all-dev libssl-dev && apt-get clean
RUN git clone -b master https://github.com/pocoproject/poco.git && cd poco && mkdir cmake-build && cd cmake-build && cmake .. && make && sudo make install && cd .. && cd ..
RUN mkdir app
ADD /home/imsemenov00/Dima/hl_mai_lab_03-main/CMakeLists.txt ./app/
RUN sudo ldconfig
RUN ls
RUN cd app && mkdir build && cd build && cmake  .. && make
ENTRYPOINT [ "app/build/messanger_api" ]
