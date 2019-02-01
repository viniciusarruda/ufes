for file in in/*
do
	splitted="$(cut -d'/' -f2 <<<"$file")"
	filename="$(cut -d'.' -f1 <<<"$splitted")"

	./trab2 < "$file" > MyOut/"$filename.out" 2>&1

	diff out2/"$filename.out" MyOut/"$filename.out"

done
