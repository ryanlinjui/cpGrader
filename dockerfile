FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    zip \
    unzip \
    curl \
    && rm -rf /var/lib/apt/lists/*

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"

# 設置工作目錄
WORKDIR /usr/src/myapp

# 複製當前目錄的內容到容器中
COPY . .

# 編譯 Rust 程式碼
RUN cargo build --release

# 執行 Rust 程式碼
CMD ["./target/release/myapp"] 