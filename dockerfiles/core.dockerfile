FROM      node:latest
WORKDIR   /app/faceless-core

COPY      . .
RUN yarn install --pure-lockfile
RUN yarn build
