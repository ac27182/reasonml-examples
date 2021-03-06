# ReasonMl Examples

## faceless

faceless is a simple, multitenant anonymous chat room application

## pre-requesites

- docker
- GNU make

## setup instructions

```sh
# 1. build all docker images
$ make build-all

# 2. run built app locally
$ make up

# 3. open http://localhost:3001 in your browser of choice

# 4. open a second browser session up in `incognito mode` to simulate a second user

# 5. hover over the orb to set your chat name

# 6. enter or make and enter a channel

# 7. send some messages

# ! see makefile for underlying build commands
```

## cleanup instructions

```sh
# 1. remove above images from local machine
$ make remove-all
```

## useful sources / documentation

### reason

- https://reasonml.github.io/docs/en/what-and-why
- https://github.com/glennsl/bs-json
- https://github.com/reazen/relude/blob/master/__tests__/Relude_IO_test.re
- https://github.com/reazen/relude
- https://bucklescript.github.io/docs/en/class
- https://bucklescript.github.io/docs/en/interop-cheatsheet
- https://dev.to/mlms13/type-classes-in-reasonml-a-world-of-functions-for-free-2lag
- https://reasonml.github.io/reason-react/docs/en/context-mixins

### misc

- https://docs.npmjs.com/files/package.json
- https://webpack.js.org/concepts/
- https://docs.nginx.com/nginx/admin-guide/web-server/serving-static-content/#
- https://github.com/docker-library/redis/blob/3a0b4f1d370e4efd30a0db32b0e0cf878f3133d3/6.0/alpine/Dockerfile
- https://www.npmjs.com/package/ws
- https://redis.js.org/
- https://classic.yarnpkg.com/en/docs/cli/install/
