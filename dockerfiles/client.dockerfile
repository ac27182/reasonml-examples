# stage 1
FROM      faceless-core:latest as stage-1
WORKDIR   /app/faceless-client

COPY      ./*json ./
COPY      ./webpack.config.js .
COPY      ./src ./src

RUN        npm install \
  && npm run build \
  && npm run webpack:production

# stage 2
FROM       nginx:alpine
WORKDIR    /build

COPY       --from=stage-1 /app/faceless-client/build .
COPY       ./nginx.conf /etc/nginx
RUN        cat /etc/nginx/nginx.conf
CMD        nginx