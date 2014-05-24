compile: main.c \
    tree.c \
    tree.h
	gcc \
	    main.c \
	    tree.c -o main
	./main
