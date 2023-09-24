cuda:
	#run befor service Docker Desktop Windows and after run this docker:
	docker run --rm -it -v /home/kimve/dev/cpp:/cpp -w /cpp/gpu_cuda kimve/cuda:12.2.0-devel-ubuntu22.04-vim /bin/bash
commit_docker:
	# You can change docker image (example install package in docker) and then commit TAG
	# SEE CONTAINER ID and image = REPOSITORY:OLD_TAG
	docker ps -a
	# COMMIT NEW TAG
	docker commit [CONTAINER ID] [REPOSITORY:[NEW_TAG]]
	# RUN NEW TAG
	docker run [REPOSITORY:NEW_TAG] 
	# PUSH to docker's repository (on docker site)
	docker push ...
debian:	
	docker run --rm -it debian:unstable-slim /bin/bash
