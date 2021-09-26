.PHONY: docker-image

v86 out/v86-linux.iso: docker-image build.sh
	bash ./build.sh

install: out/v86-linux.iso
	cp $< ../web/images


docker-image: buildroot-v86/build.sh
	docker build -t buildroot .

clean:
	docker rm buildroot
