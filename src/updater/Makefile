
GREP_FILTER = | grep -v "[???%]" | grep -v "Scanning dependencies" | grep -v "Linking CXX executable" | grep -v "Linking CXX shared library" | grep -v "Betrete Verzeichnis" | grep -v "Verlasse Verzeichnis" | grep -v "Entering directory" | grep -v "Leaving directory"

all: debug-quiet

release-quiet:
	cd ../../build/ && cmake -DCMAKE_BUILD_TYPE=Release ../ && make -j4 updater ${GREP_FILTER}

debug-quiet:
	cd ../../build/ && cmake -DCMAKE_BUILD_TYPE=Debug ../ && make -j4 updater ${GREP_FILTER}

release:
	cd ../../build/ && cmake -DCMAKE_BUILD_TYPE=Release ../ && make -j4 updater

debug:
	cd ../../build/ && cmake -DCMAKE_BUILD_TYPE=Debug ../ && make -j4 updater
