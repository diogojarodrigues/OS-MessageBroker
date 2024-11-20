.PHONY: all server user

all: server user

server:
	$(MAKE) -C src/server

user:
	$(MAKE) -C src/user

clean:
	$(MAKE) -C src/server clean
	$(MAKE) -C src/user clean
