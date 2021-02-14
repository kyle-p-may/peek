### Development environment
In order to start the development environment, run the following command:
    docker run -d -it --name peekbuild --mount type=bind,source=${PEEK_ROOT},target=/peekbuild peekbuild:latest

You can do this once when you start working, and then ssh into the container.
Ideally, the build caches would be saved between work sessions, so that is something
that we should do.