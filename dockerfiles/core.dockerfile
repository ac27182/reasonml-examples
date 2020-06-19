FROM      node:latest
WORKDIR   /app/faceless-core

COPY      ./*json ./
COPY      . .
RUN       npm install \
  && npm run build