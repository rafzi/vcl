FROM gitpod/workspace-full-vnc
USER gitpod
RUN sudo apt-get update \
 && sudo apt-get install -y ninja-build
# Software OpenGL support (LLVMpipe)
RUN sudo add-apt-repository ppa:kisak/kisak-mesa \
 && sudo apt-get update \
 && sudo apt-get install -y libgl1-mesa-dri
# OpenGL development
RUN sudo apt-get install -y libgl1-mesa-dev libglew-dev
