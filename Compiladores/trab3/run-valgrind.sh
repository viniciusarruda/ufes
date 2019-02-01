for file in in/*
do
	splitted="$(cut -d'/' -f2 <<<"$file")"
	filename="$(cut -d'.' -f1 <<<"$splitted")"

	valgrind --leak-check=full --show-leak-kinds=all ./trab3 < "$file" > out_valgrind/"$filename.out" 2>&1

done
