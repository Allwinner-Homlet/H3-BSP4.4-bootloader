include ./../config/XComplier.cfg

INCLUDES:=-I./../include

CC:=$(CCEXE) $(INCLUDES) -g -o

TARGET=fwpacker
all:$(TARGET) clean 
	
$(TARGET):$(SRCCPPS)
	$(CC) $(TARGET) $(SRCCPPS) 

.PHONY: clean
clean:
	@echo Build $(TARGET) Success!!
