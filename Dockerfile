# A Docker is necessary in order for me to able to host many different things on my Server without having to worry about another application interfering with another
#
# The code in this file is in no way monitored strictly so use at your own risk for your own hardware, I am at no responsibility for any mishaps along YOUR way
#
# Docker : LATEST
#

FROM node:latest
#FROM ubuntu as intermediate

#init git
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install -y git
RUN apt-get install npm -y

# RUN git pull
# RUN git fetch

#work dir
RUN mkdir -p /usr/src/yAPI
WORKDIR /usr/src/yAPI

#crew
ARG SSH_PRIVATE_KEY
RUN mkdir /root/.ssh/
RUN echo "${SSH_PRIVATE_KEY}" > /root/.ssh/id_rsa

RUN touch /root/.ssh/known_hosts
RUN ssh-keyscan bitbucket.org >> /root/.ssh/known_hosts

# RUN git clone git@bitbucket.org:exoad/yAPI.git

#FROM ubuntu:latest
# REMOVE COPY --from=intermediate /yAPI /srv/yAPI
#ENV TZ=America/New_York
#RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt update
RUN apt install -y tzdata

COPY package.json /usr/src/yAPI
RUN npm i

COPY . /usr/src/yAPI

RUN apt update && apt install  openssh-server sudo -y

#RUN useradd -rm -d /home/ubuntu -s /bin/bash -g root -G sudo -u 1000 test 
RUN echo test test >> new_archive.txt

#RUN  echo 'test:test' | chpasswd

#RUN service ssh start

EXPOSE 22

CMD ["node", "app.js"]
