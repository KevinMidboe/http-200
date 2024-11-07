# http-200

Simple http C server that always responds 200 to all http requests. Resulting in docker image size smaller than 40 kB (or 3 kB compressed).

## build

```bash
gcc -o server server.c
```

or using make

```bash
make
```

## run

```bash
./server
```

or

```bash
make run
```

## docker

build using docker with

```bash
docker build -t http-200 .
```

publish (multi-arch) using

```bash
make docker-publish
```
