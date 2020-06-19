# https://hub.docker.com/layers/redis/library/redis/latest/images/sha256-61ce79d60150379787d7da677dcb89a7a047ced63406e29d6b2677b2b2163e92?context=explore
redis:
	@docker run \
		--name redis-test \
		-p 6379:6379  \
		-d \
		redis:latest 

theme-list:
	@bsb -themes > themes.txt

remove-all-images:
	@docker rm -f `docker ps -aq`

redis-restart: remove-all-images redis

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


# commands to add

# brings up faceless
# faceless-up:
# 	@docker-compose up

# builds docker images for the client and server
# build-all:
	# ???

# removes built images
# clean:
# 	???