.PHONY: docker-image

v86 out/v86-linux.iso: docker-image build.sh
	bash ./build.sh

install: out/v86-linux.iso
	cp $< ../web/images


docker-image: build_docker_image_if_needed.sh Dockerfile buildroot-v86/build.sh
	./$< buildroot $(filter-out $<,$^)

clean:
	docker rm buildroot
