.PHONY: all docker-build run clean

# Variables for C compilation
CC = gcc
CFLAGS = -Os -fdata-sections -ffunction-sections -march=native -Wall -Wextra
TARGET = server

# Variables for Docker Buildx
IMAGE_NAME = kevinmidboe/http-200
VERSION = latest
PLATFORMS = linux/amd64,linux/arm64,linux/arm/v7
DATE=$(shell date +%Y.%m.%d)

# Default rule to compile the C program and build Docker image
all: $(TARGET)

run:
	./server

# Rule to compile the C executable
$(TARGET): server.c
	$(CC) $(CFLAGS) -o $(TARGET) server.c

# Docker Buildx setup
docker-setup: $(TARGET)
	docker buildx create --use --name multiarch_builder || true
	docker buildx inspect --bootstrap

# Build multi-platform Docker image without pushing to registry
docker-build: docker-setup
	docker buildx build --platform $(PLATFORMS) -t $(IMAGE_NAME):$(VERSION) -t $(IMAGE_NAME):$(DATE) .

# Build multi-platform Docker image and push to registry
docker-publish: docker-setup
	docker buildx build --platform $(PLATFORMS) -t $(IMAGE_NAME):$(VERSION) -t $(IMAGE_NAME):$(DATE) --push .

# Clean up Buildx builder and compiled binary
clean:
	rm -f $(TARGET)
	docker buildx rm multiarch_builder || true

