docker run -it \
                --net eraft-network \
                --name eraft-engine \
                --hostname eraft-engine \
                --ip 172.19.0.18 \
                -v ${PWD}:/engine \
                -p 20160:20160 \
                hub.docker.com/eraft/engine_dev:latest


# use this to update rust to new version
source $HOME/.cargo/env

rustup install nightly
# https://www.cnblogs.com/pu369/p/15161194.html
cargo +nightly bench
