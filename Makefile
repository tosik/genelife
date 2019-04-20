all: build/
	(cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make && make build-package)

build/:
	mkdir -p build

run: all
	./build/package/sdl-project-template.app/Contents/MacOS/sdl-project-template

