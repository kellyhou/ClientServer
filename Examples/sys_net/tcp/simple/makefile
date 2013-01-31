SOURCE=$(wildcard *.c)
TARGET=$(SOURCE:.c=)

all:$(TARGET)

%:%.c
	$(CC) -o $@ $^
