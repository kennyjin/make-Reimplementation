all: Build.o main.o progexec.o specgraph.o text_parsing.o
	gcc -o 537make Build.o main.o progexec.o specgraph.o text_parsing.o
Build.o: Build.c Build.h
	gcc -Wall -Wextra -c Build.c
main.o: main.c specgraph.h progexec.h text_parsing.h
	gcc -Wall -Wextra -c main.c
progexec.o: progexec.c progexec.h
	gcc -Wall -Wextra -c progexec.c
specgraph.o: specgraph.c specgraph.h progexec.h
	gcc -Wall -Wextra -c specgraph.c
text_parsing.o: text_parsing.c text_parsing.h
	gcc -Wall -Wextra -c text_parsing.c

clean:
	rm Build.o main.o progexec.o specgraph.o text_parsing.o
	rm 537make
	rm -rf scan-build-out

scan-build: clean
	scan-build -o scan-build-out make

scan-view: scan-build
	firefox -new-window scan-build-out/*/index.html