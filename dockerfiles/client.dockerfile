# stage 1
FROM faceless-core:latest as stage-1
WORKDIR /app/faceless-client

COPY *json ./
COPY yarn.lock .
RUN yarn install --pure-lockfile

COPY webpack.config.js .
COPY src src
RUN yarn run build 
RUN yarn run webpack:production

# stage 2
FROM nginx:alpine
WORKDIR /app/faceless-client/build
CMD ["nginx", "-g", "daemon off;"]

COPY --from=stage-1 /app/faceless-client/build .
COPY ./default.conf /etc/nginx/conf.d/
