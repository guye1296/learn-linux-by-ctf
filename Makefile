docker: Dockerfile
	docker image rm linux-by-ctf
	docker build -t linux-by-ctf .
