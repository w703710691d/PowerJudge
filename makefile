LD=g++
CXX=g++
CXXFLAGS=-g -Wall -O2 -DBIG_JOINS=1 -fno-strict-aliasing -DNDEBUG
LDFLAGS=
LIBS=-lbsd -L/usr/lib/x86_64-linux-gnu -lcurl -lpthread -lz -lm -ldl
TARGET=bin/powerjudge
OBJECTS=bin/judge.o
TARGETD=bin/powerjudged
OBJECTSD=bin/judged.o

ifdef LOG_LEVEL
	CXXFLAGS+= -DLOG_LEVEL=${LOG_LEVEL}
endif

ifdef FAST_JUDGE
	CXXFLAGS+= -DFAST_JUDGE
endif

.PHONY: test check install clean
all: clean $(TARGET) $(TARGETD)
$(TARGET): $(OBJECTS)
	$(LD) -o $@ $(LDFLAGS) $(OBJECTS)
	sudo chown judge:tomcat $(TARGET)
	sudo chmod 4755 $(TARGET)
	sudo setcap cap_sys_chroot+ep $(TARGET)

$(TARGETD): $(OBJECTSD)
	$(LD) -o $@ $(LDFLAGS) $(OBJECTSD) $(LIBS)
	sudo chown root:root $(TARGETD)
	sudo chmod 4755 $(TARGETD)

bin/%.o: src/%.c src/*.h
	$(CXX) -o $@ $(CXXFLAGS) -c $<

test: all
	g++ -o test/data/1405/spj test/data/1405/spj.cc
	sudo chmod a+x test/unitTest.sh
	cd test && sudo su judge -c ./unitTest.sh

check: all
	-cd src && ../bin/cpplint.py --linelength=100 --extensions=c,h \
	--filter=-whitespace/braces,-build/include \
	judge.c judge.h judged.c judged.h judge_core.h log.h misc.h syscalls.h
	-cppcheck src/judge.c  src/judged.c

install: all
	sudo cp $(TARGET) /usr/local/bin/
	sudo setcap cap_sys_chroot+ep /usr/local/bin/powerjudge
	sudo pkill -f /usr/local/bin/powerjudged
	sudo cp $(TARGETD) /usr/local/bin/
	sudo chown root:root /usr/local/bin/powerjudged
	sudo chmod 4755 /usr/local/bin/powerjudged
	sudo cp -p judge.properties /home/judge/
	sudo su judge -c /usr/local/bin/powerjudged

clean:
	sudo rm -f bin/judge.o bin/powerjudge bin/judged.o bin/powerjudged
	sudo rm -rf test/temp
