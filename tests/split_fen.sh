../print_fen --perft ../../tests/given_perft/kiwipete.perft > file
count=1; while read line; do echo "$line" > "file_$count.perft"; count=$(($count + 1)); done < file
count=$(($count - 1))
rm "file_$count.perft"
rm file
