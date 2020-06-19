FROM      faceless-core:latest
WORKDIR   /app/faceless-server

COPY      ./*json ./
COPY      . .
RUN       npm install \
  && npm run build

CMD       node src/App.bs.js
