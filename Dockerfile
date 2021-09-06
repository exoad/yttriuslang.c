FROM node:latest
FROM ubuntu as intermediate

#init git
RUN apt-get update
RUN apt-get install -y git

#work dir
RUN mkdir -p /usr/src/yAPI
WORKDIR /usr/src/yAPI

COPY package.json /usr/src/yAPI
RUN npm i

COPY . /usr/src/yAPI

CMD ["bash", "r.sh"]