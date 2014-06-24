CC = /usr/bin/gcc
GCCFLAGS = -c -Wall -g --std=c99
ROOTDIR = .
SRCDIR = $(ROOTDIR)/src
OBJDIR = $(ROOTDIR)/obj


#if using this method to find header files, the header file in c file should be included in the following way: #include "../include/header.h"
VPATH = src:obj:include 


OBJ = $(OBJDIR)/run.o \
	  $(OBJDIR)/CETD.o $(OBJDIR)/tag.o $(OBJDIR)/shuffle.o $(OBJDIR)/permutation.o  \
	  $(OBJDIR)/cipher.o  $(OBJDIR)/input.o  \
      $(OBJDIR)/aes.o  \
      $(OBJDIR)/padlock.o  \
      $(OBJDIR)/array_shift.o  \
	  $(OBJDIR)/write_txt.o \
	  $(OBJDIR)/write_csv.o \
	  $(OBJDIR)/show.o \
	  $(OBJDIR)/LL_arr_gen.o \
	  $(OBJDIR)/combination.o \
	  $(OBJDIR)/math_func.o \
	  $(OBJDIR)/gf_mult.o \

run: $(OBJ)
	$(CC) -o $@ $(OBJ) -lm
#target:prerequest
#	commend
$(OBJDIR)/run.o: $(SRCDIR)/run.c config.h show.h input.h CETD.h \
 aes.h cipher.h LL_arr_gen.h math_func.h
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/run.c 

$(OBJDIR)/CETD.o: $(SRCDIR)/CETD.c CETD.h show.h write_txt.h write_csv.h math_func.h gf_mult.h
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/CETD.c	
$(OBJDIR)/tag.o: $(SRCDIR)/tag.c tag.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/tag.c	
$(OBJDIR)/shuffle.o: $(SRCDIR)/shuffle.c shuffle.h show.h math_func.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/shuffle.c	
$(OBJDIR)/permutation.o: $(SRCDIR)/permutation.c permutation.h array_shift.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/permutation.c


$(OBJDIR)/cipher.o: $(SRCDIR)/cipher.c cipher.h write_txt.h write_csv.h
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/cipher.c	
$(OBJDIR)/input.o: $(SRCDIR)/input.c input.h write_csv.h write_txt.h
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/input.c

$(OBJDIR)/aes.o: $(SRCDIR)/aes.c config.h aes.h padlock.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/aes.c	
$(OBJDIR)/padlock.o: $(SRCDIR)/padlock.c config.h padlock.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/padlock.c
$(OBJDIR)/show.o: $(SRCDIR)/show.c show.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/show.c
$(OBJDIR)/array_shift.o: $(SRCDIR)/array_shift.c array_shift.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/array_shift.c	

$(OBJDIR)/write_txt.o: $(SRCDIR)/write_txt.c write_txt.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/write_txt.c	
$(OBJDIR)/write_csv.o: $(SRCDIR)/write_csv.c write_csv.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/write_csv.c	

$(OBJDIR)/LL_arr_gen.o: $(SRCDIR)/LL_arr_gen.c LL_arr_gen.h combination.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/LL_arr_gen.c	
$(OBJDIR)/combination.o: $(SRCDIR)/combination.c combination.h config.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/combination.c	

$(OBJDIR)/math_func.o: $(SRCDIR)/math_func.c math_func.h  
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/math_func.c	
$(OBJDIR)/gf_mult.o: $(SRCDIR)/gf_mult.c gf_mult.h math_func.h array_shift.h 
	$(CC) -o $@ $(GCCFLAGS) $(SRCDIR)/gf_mult.c	


clean:
	rm -f run $(OBJDIR)/*.o 

rebuild: clean run

