FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y \
    build-essential \      
    make \                
    unzip \                
    tar \  
    && rm -rf /var/lib/apt/lists/*

# 建立一個工作目錄，並設定該目錄為工作路徑
WORKDIR /workspace

# 預設的命令，可根據需求修改
CMD ["/bin/bash"]