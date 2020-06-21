build-core:
	@docker build \
		--no-cache \
		-f dockerfiles/core.dockerfile \
		-t faceless-core \
		faceless-core/

build-server:
	@docker build \
		--no-cache \
		-f dockerfiles/server.dockerfile \
		-t faceless-server \
		faceless-server/

build-client:
	@docker build \
		--no-cache \
		-f dockerfiles/client.dockerfile \
		-t faceless-client \
		faceless-client/

build-all: build-core build-server build-client

up: 
	@docker-compose up


remove-containers: 
	@docker rm -f $(docker ps -a | grep -wEi 'faceless-core|faceless-server|faceless-client' | awk '{print $3}')

remove-images:
	@docker rmi -f $(docker images | grep -wEi 'faceless-core|faceless-server|faceless-client' | awk '{print $3}')

clean: remove-containers remove-images

# misc
redis:
	@docker run \
		--name redis-test \
		-p 6379:6379  \
		-d \
		redis:alpine

theme-list:
	@bsb -themes > themes.txt

remove-all-images:
	@docker rm -f `docker ps -aq`

redis-restart: remove-all-images redis