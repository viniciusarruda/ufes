for file in in/*
do
	splitted="$(cut -d'/' -f2 <<<"$file")"
	filename="$(cut -d'.' -f1 <<<"$splitted")"

	valgrind ./trab4 < "$file" > valgrind_out/"$filename.out" 2>&1
	./trab4 < "$file" > out/"$filename.out" 2>&1

done
