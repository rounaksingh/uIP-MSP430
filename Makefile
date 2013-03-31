#
# Makefile for msp430
#
# 'make' builds everything
# 'make clean' deletes everything except source files and Makefile
# You need to set TARGET, MCU and SOURCES for your project.
# TARGET is the name of the executable file to be produced 
# $(TARGET).elf $(TARGET).hex and $(TARGET).txt nad $(TARGET).map are all generated.
# The TXT file is used for BSL loading, the ELF can be used for JTAG use
# 

TARGET     = main
MCU        = msp430fg4618

#
#
DEPEND_DIR= .depend
OBJECT_DIR= .object
BIN_DIR= bin
# List all the source files here
# eg if you have a source file foo.c then list it here 
SOURCES = main.c delay.c
SOURCES += net/enc28j60.c net/nic.c net/psock.c net/timer.c net/uip.c
SOURCES += net/uip_arch.c net/uip_arp.c net/uip-empty-packet.c
SOURCES += net_app/simple-app.c serial_debug/serial_debug.c
#SOURCES = main2.c delay.c serial_debug/serial_debug.c

# Include are located in the Include directory
INCLUDES = -IInclude
# Add or subtract whatever MSPGCC flags you want. There are plenty more
#######################################################################################
CFLAGS   = -mmcu=$(MCU) -g -Os -Wall -Wunused $(INCLUDES)   
ASFLAGS  = -mmcu=$(MCU) -x assembler-with-cpp -Wa,-gstabs
LDFLAGS  = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map
########################################################################################
CC       = msp430-gcc
LD       = msp430-ld
AR       = msp430-ar
AS       = msp430-gcc
GASP     = msp430-gasp
NM       = msp430-nm
OBJCOPY  = msp430-objcopy
RANLIB   = msp430-ranlib
STRIP    = msp430-strip
SIZE     = msp430-size
READELF  = msp430-readelf
MAKETXT  = srec_cat
CP       = cp -p
RM       = rm -f
MV       = mv
MKDIR    = mkdir
########################################################################################
# the file which will include dependencies
DEPEND = $(DEPEND_DIR)/$(SOURCES:.c=.d)
# all the object files
OBJECTS = $(OBJECT_DIR)/$(SOURCES:.c=.o)

make_depend_dir:
	$(MKDIR) $(DEPEND_DIR) 2>/dev/null

make_object_dir:
	$(MKDIR) $(OBJECT_DIR) 2>/dev/null

dir: make_object_dir

all: clean dir $(TARGET).elf $(TARGET).hex $(TARGET).txt

$(TARGET).elf: $(OBJECTS)
	echo "Linking "$@
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	echo
	echo ">>>> Size of Firmware <<<<"
	$(SIZE) $(TARGET).elf
	echo
%.hex: %.elf
	echo "Creating "$@
	$(OBJCOPY) -O ihex $< $@
%.txt: %.hex
	$(MAKETXT) -O $@ -TITXT $< -I
	#unix2dos $(TARGET).txt
#  The above line is required for the DOS based TI BSL tool to be able to read the txt file generated from linux/unix systems.
./$(OBJECT_DIR)/%.o: %.c
	echo "Compiling $<"
	$(CC) -c $(CFLAGS) -o $@ $<
# rule for making assembler source listing, to see the code
%.lst: %.c
	$(CC) -c $(ASFLAGS) -Wa,-anlhd $< > $@
# include the dependencies unless we're going to clean, then forget about them.
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPEND)
endif
# dependencies file
# includes also considered, since some of these are our own
# (otherwise use -MM instead of -M)
./$(DEPEND_DIR)/%.d: %.c make_depend_dir
	echo "Generating dependencies $@ from $<"
	$(CC) -M ${CFLAGS} $< > $@
.SILENT:
.PHONY:	clean
clean:
	echo Cleaning Up
	-$(RM) $(OBJECTS)
	-$(RM) $(TARGET).hex
	-$(RM) $(TARGET).eep
	-$(RM) $(TARGET).cof
	-$(RM) $(TARGET).elf
	-$(RM) $(TARGET).map
	-$(RM) $(TARGET).sym
	-$(RM) $(TARGET).lss
	-$(RM) $(TARGET).txt
	-$(RM) $(SOURCES:.c=.lst)
	-$(RM) $(DEPEND)
	-$(RM) -r $(DEPEND_DIR) $(OBJECT_DIR)
