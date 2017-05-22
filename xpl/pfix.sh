#!/bin/bash

PROJDIR="."
TESTDIR="../testeXPL/tests-xpl-daily-201701121739/"
ASMDIR="./asm"
BINDIR="./bin"
BUILDDIR="./build"
EXPECTED=`mktemp`

set -e

rm -rf "$ASMDIR" "$BINDIR" "$BUILDDIR"
mkdir -p "$ASMDIR" "$BINDIR" "$BUILDDIR"

(cd "$PROJDIR" && make clean && make)

for i in "$TESTDIR"/*.xpl
do
    TEST=${i%.xpl}
    TEST=${TEST##*/}
    ASM="$ASMDIR"/"$TEST".asm
    OBJ="$BUILDDIR"/"$TEST".o
    BIN="$BINDIR"/"$TEST"
    cat "$TESTDIR"/expected/"$TEST".out | tr '\n' ' ' > "$EXPECTED"
    # clear
    echo "Processing $TEST..."
    $PROJDIR/xpl "$i" -o "$ASM"
    yasm -felf32 "$ASM" -o "$OBJ"
    ld -melf_i386 -o "$BIN" "$OBJ" -lrts
    echo "Here's the program:"
    echo "--------------------"
    cat "$i"
    echo "--------------------"
    echo ""
    "$BIN" | tr '\n' ' ' | colordiff -u --ignore-all-space - "$EXPECTED" -w 
    # echo "Here's the output:"
    # "$BIN"
    # echo
    # echo "--------------------"
    # echo "Here's the expected output:"
    # cat "$TESTDIR"/expected/"$TEST".out
    # echo "--------------------"
    # read x
done
echo "Congratulations! All tests are done!"
