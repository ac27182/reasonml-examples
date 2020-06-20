FROM faceless-core:latest as stage-1
WORKDIR /app/faceless-server
CMD ["node", "src/App.bs.js"]

COPY *json ./
COPY yarn.lock .

RUN yarn install --pure-lockfile
RUN yarn --cwd ../faceless-core install --pure-lockfile

COPY . .
RUN yarn run build
RUN yarn --cwd ../faceless-core run build
