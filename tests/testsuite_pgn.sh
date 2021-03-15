# !/bin/sh

for filename in "../tests/given_pgn/"*".pgn"
do
    filename="${filename%.*}"
    echo "----------- $filename --------------"
    ./chessengine --pgn "$filename.pgn" -l "../tests/libbasic-output.so" > /tmp/actual
    diff -u "$filename.out" /tmp/actual
done
