all: debug release

debug: build/debug
	(cd build/debug && cmake ../../ -DCMAKE_BUILD_TYPE=Debug && make && make build-package)

release: build/release
	(cd build/release && cmake ../../ -DCMAKE_BUILD_TYPE=Release && make && make build-package)

build/release:
	mkdir -p build/release

build/debug:
	mkdir -p build/debug

run: all
	./build/release/package/genelife.app/Contents/MacOS/genelife

