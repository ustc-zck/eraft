default: image

BUILDER_IMAGE := $(or $(BUILDER_IMAGE),hub.docker.com/eraft/engine_dev)

image:
	docker build -f Dockerfile -t $(BUILDER_IMAGE) .

dev:
	docker run -it --net eraft-network --name eraft-engine --hostname eraft-engine --ip 172.19.0.18 -v $(realpath ..):/engine -p 20160:20160 hub.docker.com/eraft/engine_dev:latest

clean:
	docker rm $(docker ps -aq)
