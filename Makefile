all: build/
	(cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make && make build-package)

build/:
	mkdir -p build

run: all
	./build/package/genelife.app/Contents/MacOS/genelife

