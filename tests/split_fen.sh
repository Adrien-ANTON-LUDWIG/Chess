../chessengine --perft ../../tests/given_perft/provided_8.perft > file
count=1; while read line; do echo "$line" > "file_$count.perft"; count=$(($count + 1)); done < file
