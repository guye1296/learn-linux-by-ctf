docker: Dockerfile
	docker build -t linux-by-ctf .

clean:
	docker image rm linux-by-ctf

