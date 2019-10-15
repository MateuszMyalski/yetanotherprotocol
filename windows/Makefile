# -*- MakeFile -*-
CC = gcc
LIBPATH = YetAnotherProtocol
BUILDPATH = build
OUTFILENAME = mainProgram
FLAGS = -Wall
all: main
	@echo =====================STARTING PROGRAM=====================
	$(OUTFILENAME).exe
	@echo ======================ENDING PROGRAM======================
	

main: main.o YAP_user_API.o YAP_send_packet.o YAP_receive_packet.o YAP_abstract_hardware.o YAP_process_byte.o
	$(CC) $(FLAGS) -o $(OUTFILENAME) $(BUILDPATH)/main.o $(BUILDPATH)/YAP_user_API.o $(BUILDPATH)/YAP_send_packet.o $(BUILDPATH)/YAP_receive_packet.o $(BUILDPATH)/YAP_abstract_hardware.o $(BUILDPATH)/YAP_process_byte.o 

main.o: main.c
	$(CC) $(FLAGS) -c main.c -o $(BUILDPATH)/main.o

YAP_user_API.o: $(LIBPATH)/YAP_user_API.c
	$(CC) $(FLAGS) -c $(LIBPATH)/YAP_user_API.c -o $(BUILDPATH)/YAP_user_API.o

YAP_send_packet.o: $(LIBPATH)/YAP_send_packet.c
	$(CC) $(FLAGS) -c $(LIBPATH)/YAP_send_packet.c -o $(BUILDPATH)/YAP_send_packet.o

YAP_receive_packet.o: $(LIBPATH)/YAP_receive_packet.c
	$(CC) $(FLAGS) -c $(LIBPATH)/YAP_receive_packet.c -o $(BUILDPATH)/YAP_receive_packet.o

YAP_abstract_hardware.o: $(LIBPATH)/YAP_abstract_hardware.c
	$(CC) $(FLAGS) -c $(LIBPATH)/YAP_abstract_hardware.c -o $(BUILDPATH)/YAP_abstract_hardware.o

YAP_process_byte.o: $(LIBPATH)/YAP_process_byte.c
	$(CC) $(FLAGS) -c $(LIBPATH)/YAP_process_byte.c -o $(BUILDPATH)/YAP_process_byte.o

clean:
	del -f $(BUILDPATH)\*.o
	del -f $(OUTFILENAME).exe