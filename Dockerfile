# FROM alpine:latest as build
# RUN apk add build-base cmake git
# COPY . /var/app
# WORKDIR /var/app/build
# RUN cmake -D CMAKE_EXE_LINKER_FLAGS="-static" .. && make

# FROM alpine:latest
# WORKDIR /var/app
# COPY --from=build /var/app/build/webserver-highload .
# ENTRYPOINT ["./webserver-highload"]

FROM gcc:11
RUN apt-get update && apt-get install -y cmake libspdlog-dev
COPY . /var/app
WORKDIR /var/app/build
RUN cmake .. && make
ENTRYPOINT ["./webserver-highload"]
