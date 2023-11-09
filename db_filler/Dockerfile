# syntax=docker/dockerfile:1
FROM ubuntu

RUN apt-get clean && apt-get update -y
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install cmake gcc git g++ python3 python3-pip && apt-get clean

WORKDIR /app

COPY requirements.txt requirements.txt
RUN pip3 install -r requirements.txt

COPY . .

RUN mkdir build && cd build && cmake .. && make

CMD ["python3", "main.py"]
