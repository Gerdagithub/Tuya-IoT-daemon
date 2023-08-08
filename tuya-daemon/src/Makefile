BIN:=tuya_daemon
SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)
CFLAGS:=-Wall
CPPFLAGS:=-I$(CURDIR) -I/home/studentas/Documents/rutx_src/build_dir/target-arm_cortex-a7+neon-vfpv4_musl_eabi/lua-5.1.5/src/ -I/usr/lib/x86_64-linux-gnu/lua/5.1/ \
 					-I/home/studentas/Documents/rutx_src/build_dir/target-arm_cortex-a7+neon-vfpv4_musl_eabi/toolchain/ipkg-arm_cortex-a7_neon-vfpv4/libc

LDLIBS:=-llink_core \
        -lmiddleware_implementation \
		-lplatform_port \
		-lutils_modules \
		-lblobmsg_json -lubox -lubus \
		-largp -llua

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) $(OBJS) $(BIN)