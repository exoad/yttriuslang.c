FROM node:latest
FROM ubuntu as intermediate

#init git
RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt-get install -y git
RUN apt-get install openssh-server -y

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

FROM ubuntu
COPY --from=intermediate /yAPI /srv/yAPI

COPY package.json /usr/src/yAPI
RUN npm i

COPY . /usr/src/yAPI

CMD ["node", "app.js"]
