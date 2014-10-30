DRIVERS = roboclaw-driver ninedof-driver maestro-driver location-driver
SUBDIRS = common $(DRIVERS) 

all: 
	-for dir in $(SUBDIRS); do (cd $$dir; $(MAKE) ); done

clean:
	-for dir in $(SUBDIRS); do (cd $$dir; $(MAKE) clean); done

.PHONY: clean
