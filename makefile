CC = gcc
TEX = platex


CFLAGS = -Wall $(COPTIONS)

LDFLAGS = 
LOADLIBES =  
LDLIBS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -L/usr/local/lib -I/usr/local/include/SDL2 -Wl,-rpath,/usr/local/lib

TARGET = blackship
OBJS = $(TARGET).o actionwindow.o mainwindow.o partswindow.o calcparts.o calculation.o filesystem.o
HEADS = blackship.h

# 課題に取り組むにはこのターゲットで
#   -g3 はデバッグ情報を埋め込むオプション
#
# ターゲット指定しないと，最初のターゲットであるこれが選ばれる
#
debug: CPPFLAGS = 
debug: COPTIONS = -g3
debug: $(TARGET)

# デバッグ情報を埋め込まないターゲット
#   -O3 は最適化オプション
#
# ゲームの個人開発などで，完成品をmakeする際などに使うとよい
#
release: CPPFLAGS = -DNDEBUG
release: COPTIONS = -O3 
release: $(TARGET)

# 暗黙のルールでの動作は以下の通り
# cソースからオブジェクト
#   $(CC) $(CFLAGS) $(CPPFLAGS) -c n.c -o n.o
# オブジェクトから実行ファイル
#   $(CC) $(LDFLAGS) n.o $(LOADLIBES) $(LDLIBS) -o n
#
$(TARGET): $(OBJS)
$(OBJS): $(HEADS)

# レポート作成用ターゲット
# 例えば repo.tex があるとき，
#  make repo.pdf でpdfまで生成する
#
# texからdviへは暗黙のルール $(TEX) n.tex が使われる
#
%.pdf:%.dvi
	dvipdfmx $<

# コンパイルをやり直したい時などに，
# 中間ファイルなどを削除するターゲット
#   使用例： make clean
#
.PHONY: clean
clean:
	@rm -f $(TARGET) $(OBJS) core *~
