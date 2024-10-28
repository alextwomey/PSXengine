TARGET = psxeg
TYPE = ps-exe

SRCS = \
third_party/nugget/common/crt0/crt0.s \
program/main.c \
program/constants.c \
program/readFromCD.c \
program/2D.c \
program/pad.c \
program/SPU.c \
program/3D.c  \

CPPFLAGS += -Ithird_party/psyq-iwyu/include
LDFLAGS += -Lthird_party/psyq/lib
LDFLAGS += -Wl,--start-group
LDFLAGS += -lapi
LDFLAGS += -lc
LDFLAGS += -lc2
LDFLAGS += -lcard
LDFLAGS += -lcd
LDFLAGS += -lcomb
LDFLAGS += -lds
LDFLAGS += -letc
LDFLAGS += -lgpu
LDFLAGS += -lgs
LDFLAGS += -lgte
LDFLAGS += -lgun
LDFLAGS += -lhmd
LDFLAGS += -lmath
LDFLAGS += -lmcrd
LDFLAGS += -lmcx
LDFLAGS += -lpad
LDFLAGS += -lpress
LDFLAGS += -lsio
LDFLAGS += -lsnd
LDFLAGS += -lspu
LDFLAGS += -ltap
LDFLAGS += -Wl,--end-group
LDFLAGS += -lgcc
CFLAGS += -fpermissive

include third_party/nugget/common.mk
