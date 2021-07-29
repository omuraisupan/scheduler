all: clean scheduler

scheduler: scheduler_main.o scheduler.o
	gcc obj/scheduler_main.o obj/scheduler.o -o bin/scheduler 

scheduler_main.o: src/scheduler_main.c
	gcc -c src/scheduler_main.c
	mv scheduler_main.o obj/

scheduler.o: src/scheduler.c
	gcc -c src/scheduler.c
	mv scheduler.o obj/

clean:
	rm -f bin/scheduler obj/scheduler.o obj/scheduler_main.o