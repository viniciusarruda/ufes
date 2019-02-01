for file in in/*
do
	splitted="$(cut -d'/' -f2 <<<"$file")"
	filename="$(cut -d'.' -f1 <<<"$splitted")"

	./trab3 < "$file" > out/"$filename.out" 2>&1

	# diff out3/"$filename.out" MyOut/"$filename.out"

done
