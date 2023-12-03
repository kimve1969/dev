grpc:
	docker run --rm -it -v /exchange:/exchange -v /home/kimve/dev:/home/kimve/dev -w /home/kimve/dev --name kimve-grpc kimve/ubuntu-grpc:latest /bin/bash
cuda:
	#run befor service Docker Desktop Windows and after run this docker:
	docker run --rm -it -v /home/kimve/dev/cpp:/cpp -w /cpp/gpu_cuda kimve/cuda:12.2.0-devel-ubuntu22.04-vim /bin/bash
gpu_cuda_01:
	docker run --rm -v /home/kimve/dev/cpp:/cpp -w /cpp/gpu_cuda kimve/cuda:12.2.0-devel-ubuntu22.04-vim make 01
commit_and_push_docker:
	# You can change docker image (example install package in docker) and then commit TAG
	# SEE CONTAINER ID and image = REPOSITORY:OLD_TAG
	docker ps -a
	# COMMIT NEW TAG
	# docker commit 7261736ffgf kimve/ubuntu-grpc:latest
	docker commit [CONTAINER ID] [REPOSITORY:[NEW_TAG]]
	# RUN NEW TAG
	docker run [REPOSITORY:NEW_TAG]
	# login 
	docker login -u kimve -p my_password docker.io
	# secure login
	docker login -u kimve --password-stdin docker.io	
	# PUSH to docker's repository (on docker site)
	docker push [REPOSITOTY:TAG]
install_docker:
	# see https://losst.pro/ustanovka-docker-v-debian-11
