
GREP_FILTER = | grep -v "[???%]" | grep -v "Scanning dependencies" | grep -v "Linking CXX executable" | grep -v "Linking CXX shared library" | grep -v "Betrete Verzeichnis" | grep -v "Verlasse Verzeichnis" | grep -v "Entering directory" | grep -v "Leaving directory"

# there is a bug in boost python and g++ 4.7. Try to remove the specific compiler flag at some point in the future

all: debug-quiet

release-quiet:
	cd ../../build/ && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=/usr/bin/g++-4.6 ../ && make -j4 fluorescence ${GREP_FILTER}

debug-quiet:
	cd ../../build/ && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/g++-4.6 ../ && make -j4 fluorescence ${GREP_FILTER}

release:
	cd ../../build/ && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=/usr/bin/g++-4.6 ../ && make -j4 fluorescence

debug:
	cd ../../build/ && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/g++-4.6 ../ && make -j4 fluorescence

