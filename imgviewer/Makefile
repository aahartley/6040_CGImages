

OFILES = \
         base/Matrix.o \
         base/LinearAlgebra.o \
		 base/ImgProc.o \
		 base/ImageProcessing.o\
		 base/Mandelbrot.o\
		 base/ImageStatistics.o\
		 base/StarterViewer.o \
		 base/ImgViewer.o \

		  




ROOTDIR = .
LIB = $(ROOTDIR)/lib/libstarter.a 


GLLDFLAGS     = -lglut -lGL -lm -lGLU 

CXX = g++ -Wall -g -O2 -fPIC  $(DEFINES) -fopenmp -std=c++14 
PYTHONINCLUDE = -I/usr/include/python3.8
SWIGLD = $(CXX) -shared
SWIGEXEC = swig


INCLUDES =  -I ../build/include -I ./include/ -I /usr/local/include/ -I/usr/include/ $(PYTHONINCLUDE)




.C.o:
	$(CXX) -c $(INCLUDES) $< -o $@

base: $(OFILES)
	ar rv $(LIB) $?
	$(SWIGEXEC) -c++ -python -shadow -I./include/ swig/StarterViewer.i
	$(CXX) -c swig/StarterViewer_wrap.cxx  $(INCLUDES) -o swig/StarterViewer_wrap.o
	$(SWIGLD) swig/StarterViewer_wrap.o  $(SWIGLIB) -ldl -L./lib -lstarter $(GLLDFLAGS)  -o swig/_StarterViewer.so
	cp swig/StarterViewer.py python/
	cp swig/_StarterViewer.so python/
	$(CXX) base/Main.C $(INCLUDES)  -L./lib  -lstarter $(GLLDFLAGS)  -L ../build/lib -lOpenImageIO -lOpenImageIO_Util -o bin/imgviewer

clean:
	rm -rf bin/imgviewer doc/html *.o base/*.o base/*~ include/*~ python/*~ $(LIB)  *~ swig/*.cxx swig/*~ swig/*.so swig/*.o swig/StarterViewer.py swig/*.pyc ./*.pyc python/*StarterViewer*

.PHONY: doc
doc:
	cd doc;doxygen Doxyfile


