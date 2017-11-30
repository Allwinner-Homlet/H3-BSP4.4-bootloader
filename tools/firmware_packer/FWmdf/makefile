include ./../config/XComplier.cfg

INCLUDES:=-I ./../include

CC:=$(CCDLL) -D_IMG_64 $(INCLUDES) -g -o

TARGET=FWmdf.so
all:$(TARGET) clean 
	
$(TARGET):$(SRCCPPS)
	$(CC) $(TARGET) $(SRCCPPS) 

.PHONY: clean
clean:
	@echo Build $(TARGET) Success!!
	
