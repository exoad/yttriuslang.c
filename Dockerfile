FROM node:latest

RUN mkdir -p /usr/src/yAPI
WORKDIR /usr/src/yAPI

COPY package.json /usr/src/yAPI
RUN npm i

COPY . /usr/src/yAPI

CMD ["node", "app.js"]