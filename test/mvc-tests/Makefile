all: compile check


compile:
	../mvc test.mv | tee mvc.log

check:
	grep "syntax error" mvc.log

clean:
	$(RM) *.log


