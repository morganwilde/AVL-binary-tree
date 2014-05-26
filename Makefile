compile: main.c \
    tree.c \
    tree.h \
    database.c \
    database.h
	gcc \
	    main.c \
	    tree.c \
	    database.c -o main
	./main
	cat data/elements.db
	rm data/elements.db
