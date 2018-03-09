FROM node:7.7.2

WORKDIR /cache
ADD package.json package.json
ADD package-lock.json package-lock.json
RUN npm i

WORKDIR /app
ADD . .
RUN cp -a /cache/node_modules node_modules
RUN npm run build


CMD node .

