HGPUSHURL=ssh://hg@bitbucket.org/enhering/libstuff
HGURL=https://enhering@bitbucket.org/enhering/libstuff

BINDIR=bin/
OBJDIR=bin/

CLASSES_SRC_DIR=src/classes
EXEC_SRC_DIR=src/

ifneq ($(wildcard FREEBSD),)
  OS=FREEBSD
endif
ifneq ($(wildcard MACOS),)
  OS=MACOS
endif
ifneq ($(wildcard RASPBIAN),)
  OS=RASPBIAN
endif
ifneq ($(wildcard DEBIAN),)
  OS=DEBIAN
endif

ifeq ($(OS),MACOS)
  COMPILER=llvm-g++ -w 

  ICGICC=-I/usr/include
  LCGICC=-L/usr/lib -L/Users/enhering/tmp/cgicc-3.2.12/cgicc/.libs -lcgicc

  IOPENCV=-I/usr/local/include/opencv -I/usr/local/include
  LOPENCV=-L/usr/local/Cellar/opencv/3.4.0_1/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_face -lopencv_photo -lopencv_fuzzy -lopencv_img_hash -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ml -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_xobjdetect -lopencv_imgcodecs -lopencv_objdetect -lopencv_xphoto -lopencv_imgproc -lopencv_core

  IBOOST=-I/usr/local/Cellar/boost/1.64.0_1/include/
  LBOOST=-L/usr/local/Cellar/boost/1.64.0_1/lib/ -lboost_regex-mt

  IROOT=-stdlib=libc++ -std=c++11 -m64 -I/Applications/root_v6.12.06/include
  LROOT=-Wl,-rpath,/Applications/root_v6.12.06/lib -L/Applications/root_v6.12.06/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lpthread -stdlib=libc++ -lm -ldl
  
  LCURL=-lcurl

  IMYSQLCPPCONN=-I/opt/local/include/
  LMYSQLCPPCONN=-L/opt/local/lib -lmysqlcppconn

  ICRYPTOPP=-I/opt/local/include/
  LCRYPTOPP=-L/opt/local/lib -lcryptopp

  DOCUMENT_ROOT=~/Sites/mms
  CGI_BIN=~/Sites/

  IJSON=-I/Users/enhering/tmp/libjson
  LJSON=-L/Users/enhering/tmp/libjson -ljson

  IXML=-I/usr/local/Cellar/libxml2/2.9.3/include/libxml2/
  LXML=-L/usr/local/Cellar/libxml2/2.9.3/lib/libxml2.a -lxml2

  LNCURSES=-lncurses 
endif

ifeq ($(OS),DEBIAN)
  COMPILER=g++ -w
  ICGICC=-I/usr/local/include/
  LCGICC=-L/usr/local/lib -lcgicc

  IOPENCV=-I/usr/local/include/opencv -I/usr/local/include
  LOPENCV=-L/usr/local/lib -lopencv_dnn -lopencv_ml -lopencv_objdetect -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core
 
  IROOT=-pthread -std=c++11 -m32 -msse -mfpmath=sse -I/usr/local/root/include
  LROOT=-L/usr/local/root/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic 

  LCURL=-lcurl

  IBOOST=-I/usr/local/include
  LBOOST=-L/usr/local/lib -lboost_regex-mt

  IMYSQLCPPCONN=-I/usr/local/include/
  LMYSQLCPPCONN=-L/usr/local/lib -l mysqlcppconn

	ICRYPTOPP=-I/opt/local/include/
  LCRYPTOPP=-L/opt/local/lib -lcryptopp

  IXML=-I/usr/local/include/libxml2
  LXML=-L/usr/local/lib -lxml2

  DOCUMENT_ROOT=/usr/local/www/apache24/data/
  CGI_BIN=/usr/local/www/apache24/data/

  IJSON=-I~/tmp/libjson
  LJSON=-L~/tmp/libjson -ljson

  LNCURSES=-lncurses 
endif

INCLUDES=-Isrc/classes $(IOPENCV) $(IROOT)

CLASSES=$(CLASSES_SRC_DIR)/Base.h      $(CLASSES_SRC_DIR)/Base.cpp     \
        $(CLASSES_SRC_DIR)/NIST.h         $(CLASSES_SRC_DIR)/NIST.cpp 
DESKTOP_EXECUTABLES=$(EXEC_SRC_DIR)/LIBStuff.cpp   $(EXEC_SRC_DIR)/LIBStuff.h

SOURCES=$(CLASSES) $(DESKTOP_EXECUTABLES)


all: $(BINDIR)/LIBStuff   $(CLASSES_SRC_DIR)/BuildNumber.h

$(OBJDIR)/LIBStuff: $(EXEC_SRC_DIR)/LIBStuff.cpp \
	                  $(EXEC_SRC_DIR)/LIBStuff.h   \
	                  $(OBJDIR)/NIST.o
	@echo 'LIBStuff'
	@$(COMPILER) $(EXEC_SRC_DIR)/LIBStuff.cpp  \
	            $(OBJDIR)/NIST.o              \
	            $(OBJDIR)/Base.o              \
              $(INCLUDES) $(LROOT) $(LCURL) \
              -o $(BINDIR)/LIBStuff

$(OBJDIR)/NIST.o: $(CLASSES_SRC_DIR)/NIST.cpp \
	                $(CLASSES_SRC_DIR)/NIST.h   \
	                $(OBJDIR)/Base.o

	@echo 'NIST.o'
	@$(COMPILER) -c $(CLASSES_SRC_DIR)/NIST.cpp $(INCLUDES) -o $(OBJDIR)/NIST.o

$(OBJDIR)/Base.o: $(CLASSES_SRC_DIR)/Base.cpp \
	                $(CLASSES_SRC_DIR)/Base.h 
	@echo 'Base.o'
	@$(COMPILER) -c $(CLASSES_SRC_DIR)/Base.cpp $(INCLUDES) -o $(OBJDIR)/Base.o

# rule for build number generation
$(CLASSES_SRC_DIR)/BuildNumber.h: $(SOURCES)
	@sh make_buildnum.sh
	@echo Generated new build number.

clean:
	@rm -rf bin/*

push: 
	@hg push $(HGPUSHURL)
	@echo "Pushed current version to repo."

pull: 
	@hg pull -u $(HGURL)
	@echo "Pulled from repo and updated."
