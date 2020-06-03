
# https://hub.docker.com/layers/redis/library/redis/latest/images/sha256-61ce79d60150379787d7da677dcb89a7a047ced63406e29d6b2677b2b2163e92?context=explore
redis:
	@docker run \
		--name redis-test \
		-p 6379:6379  \
		-d \
		redis:latest 