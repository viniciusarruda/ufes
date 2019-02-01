
Nota sobre o tamanho deste README:
	
	Comentar o código ia ficar muito ruim pois eu teria que explicar o meu raciocínio nos comentários, 
	porém eles ficariam muito embolados com o código pois seriam grandes, portanto, segue abaixo algumas 
	explicações que deveriam estar comentadas no código.


1) Como executar:

Na raíz do trabalho digite o seguinte comando:

	$ make

Uma versão de debug pode ser compilada com o seguinte comando:

	$ make debug

Esta opção gera um executável que imprime a pilha quando:

	- É criado a pilha.
	- É feito um push.
	- É feito um pop.
	- É feito um store.
	- É deletado a pilha.

Caso haja input do usuário ao programa em execução ou output do programa, este será feito 
em meio as impressões da pilha, portanto pode ser que não fique legível nestes casos.


2) Arquivo run.sh:

Foi incluído um script para executar todos os arquivos, com e sem o uso do valgrind, 
na pasta in e colocar a respectiva saída na pasta out e na pasta valgrind_out. 
Uso:
	$ bash run.sh

As pastas out e valgrind_out estão sendo enviadas com minhas saídas geradas com o executável na versão 
normal (sem debug) e os programas que solicitaram input do usuário, inseri valores aleatórios.
O script irá sobrescrever os arquivos de mesmo nome dentro destas pastas.


3) Em relação ao trabalho 3:

Percebi que seria muito complicado fazer o trabalho 4 em cima do meu trabalho 3, portando refiz o trabalho 3 
de maneira mais genérica, o que tornou muito mais simples fazer o trabalho 4 apesar de ficar bem menos organizado 
e legível.


4) INTERPRETER ERROR:

Tomei a liberdade de implementar alguns erros de interpretação e são eles:

	- Ao pegar algo do usuário, e nada é digitado.
	- Ao pegar uma string do usuário sendo que era para ser int.
	- Atribuição de arrays sem acesso com índice. (Execute com o arquivo err3_14.cm)
	- Uso da função output passando array sem acesso com índice. (Execute com o arquivo err3_15.cm)
	- Função que retorna void porém era esperado int em uma atribuição. (ex: a = f(); sendo f uma função que retorna void)
	- Uso do return retornando valor diferente do declarado na função. (ex: return; mas função foi declarada retornando int)
	- Função main não definida.
	- Função main definida com argumentos diferente de void.
	- Função main definida com valor de retorno diferente de void.


5) Fiz o trabalho, depois li a especificação:

Pois é, fiz o trabalho e somente depois fui ler a especificação, e vi que no item 3 foi comentado como deveria 
ser feito o trabalho. Bom, o meu trabalho não faz exatamente o que é descrito no item. As diferenças são:

	- Ordem de empilhamento dos argumentos:

		É dito para empilhar os argumentos em uma ordem e desempilhar em ordem inversa nas chamadas de função.
	Eu até comecei fazendo isso, porém encontrei uma forma mais simples devido a funções já implementadas que eu 
	possuia. Os argumentos são empilhados em uma ordem, e na chamada da função eles são "desempilhados" na mesma ordem. Porque das áspas ? Não são desempilhados (veja próximo item).

	- Apenas uma área de memória:

		Me baseei no que aprendi durante o curso, tudo está em uma única memoria, porem tudo é dividido em 
	segmentos. O código deve estar no segmento de código, no caso do trabalho é a AST. As variáveis são empilhadas 
	a cada chamada de função, portanto todas as variáveis, argumentos, operações, frame pointers e outros que não 
	são relativos à percorrer a AST ou dados na tabela de símbolos está na pilha, com apenas uma única exceção: a 
	tabela de símbolos armazena o offset da respectiva variável com relação ao frame pointer. 

	- Array: 

		A tabela de símbolos armazena o offset da célula que contém como valor o offset do início do array, exemplo:

			int x;
			int a[3];
			x = 4;

		Temos como resultado a pilha com os seguintes dados:

               fp  x   a  a[0]  a[1]  a[2]
		STACK: 0   4   3   0     0     0 

		Note que 'a' contém 3, e a[0] é exatamente o índice 3 + 0 da pilha (que começa com o índice zero).

	- Escalar:

		Como dito acima, o valor do escalar está na pilha e é armazenado na tabela de símbolos o offset da pilha a
		partir do frame pointer.

	- Função:

		Chamada:
			1) Se a função tiver retorno, é empilhado uma célula para armazenamento do valor.
			2) Argumentos a serem passados na chamada são empilhados da esquerda para a direita.
			3) É consultado na tabelade símbolos o ponto de execução da função na AST e em seguida executado.

		Na função:
		  	1) Empilha frame pointer.
			2) Os argumentos são declarados, guardando na tabela de símbolos os respectivos offsets relativos 
			   à cada argumento passado.
			3) Variáveis no início do bloco são declaradas, sendo empilhados células e o offset armazenado na tabela 
			   de símbolos. No caso do array, a sua respectiva célula vai conter o offset real dele, sendo inserido 
			   na tabela de símbolos, sendo assim, uma referência.
			4) Executa recursivamente os statements.
			5) As variáveis são desempilhadas.
			6) Se tiver valor de retorno, o valor é armazenado no offset: frame pointer - número de argumentos - 1, 
			   que é a célula empilhada na chamada.
			7) Desempilha frame pointer, o atualizando com o valor desempilhado.
			8) O ponto de execução volta para quem chamou esta função.

		No retorno:
			1) Desempilha os argumentos passados como parâmetro.

		Note que caso tenha retorno, a célula com o valor de retorno é deixada na pilha para consumo futuro. A célula 
		de retorno foi empilhada no local correto, sem ter que realocá-la após o fim da chamada da função.


	- stdin:

		Utilizei freopen() para reabrir o stdin ao invés de fopen() como havia sugerido.


6) Memory Leak:

Como nos trabalhos anteriores, o trabalho 4 também possui vazamento de memória, 
porém este vazamento não é de parte minha e sim da ferramenta Flex. 

Por algum motivo, o Flex não limpa toda a memória utilizada, o total de memória vazada é sempre 
16.458 bytes em 3 blocos e o vilgrind acusa como "still reachable". Alguns dizem que memória vazando 
porém ainda alcançável no final do programa não é realmente vazamento 
(https://stackoverflow.com/questions/40227135/why-yacc-have-memory-leak-at-exit)
porém me sinto desconfortável, logo encontrei a seguinte solução:

	yylex_destroy()       

No entanto, não consegui compilar sem warning (implicit declaration of function ‘yylex_destroy’), portanto
deixei ela comentada pois não me senti seguro em deixá-la apesar de ter liberado toda a memória após sua inclusão, 
logo o trabalho 4 ainda possui vazamento de memória devido ao Flex.

Fonte:
http://sector7.xray.aps.anl.gov/~dohnarms/programming/flex/html/Init-and-Destroy-Functions.html

http://harveyserv.ath.cx/yylex_destroy-is-well-hidden/

https://books.google.com.br/books?id=3Sr1V5J9_qMC&pg=PA210&lpg=PA210&dq=flex+yylex_destroy&source=bl&ots=WFNzmbnOLR&sig=8_Bw7vG5HN014-h-Y24CT24oxSs&hl=pt-BR&sa=X&ved=0ahUKEwjDqMCT0IfVAhWFg5AKHUINAZgQ6AEISjAE#v=onepage&q=flex%20yylex_destroy&f=false


7) Observações finais:

	- Todos os valores são inicializados com zero.
	- A impressão de string suporta nova linha ('\n').
	- Caso haja algum erro, por favor o comente para que eu possa corrigir e entender o motivo, contribuindo 
	  para meu conhecimento. Me envie também os dados de entrada utilizados para que eu possa replicar o erro.
	- E por fim, peço desculpas pelo código deselegante e ilegível (principalmente o interpreter.c).
