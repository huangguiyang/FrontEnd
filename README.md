
A front-end described in 《Compilers: Principles, Techniques and Tools (Second Edition)》, C++ version source code.

NOTE: 

1. It's a demo, so *no* memory management, all 'new', 'new', 'new', no 'delete'.
2. Using asprintf/vasprintf, so only supports GNU or BSD operating systems. (Linux/OS X)
3. Only tested in Mac OS X 10.9.

Syntax Description
-------------------------------------------------------------------------------------
	program     ->      block
	block       ->      { decls stmts }
	decls       ->      decls decl | $
	decl        ->      type id ;
	type        ->      type [ num ] | basic
	stmts       ->      stmts stmt | $


	stmt        ->      loc = bool ;
        |   if ( bool ) stmt
        |   if ( bool ) stmt else stmt
        |   while ( bool ) stmt
        |   do stmt while ( bool ) ;
        |   break ;
        |   block
        
	loc         ->      loc [ bool ] | id
	bool        ->      bool || join | join
	join        ->      join && equality | equality
	equality    ->      equality == rel || equality != rel | rel
	rel         ->      expr < expr | expr <= expr | expr >= expr | expr > expr | expr
	expr        ->      expr + term | expr - term | term
	term        ->      term * unary | term / unary | unary
	unary       ->      ! unary | - unary | factor
	factor      ->      ( bool ) | loc | num | real | true | false



Example Input:
-------------------------------------------------------------------------------------
	{
		int i; int j; float v; float x; float[100] a;
	
		while (true) {
	
			do i = i+1; while (a[i] < v);
			do j = j+1; while (a[j] > v);
	
			if (i >= j) break;
	
			x = a[i]; a[i] = a[j]; a[j] = x;
		}
	
	}
	


Example Output:
-------------------------------------------------------------------------------------
	L1:L3:	i = i + 1
	L5:     t1 = i * 8
	        t2 = a [t1]
	        if t2 < v goto L3
	L4:     j = j + 1
	L7:     t3 = j * 8
	        t4 = a [t3]
	        if t4 > v goto L4
	L6:     iffalse i >= j goto L8
	L9:     goto L2
	L8:     t5 = i * 8
	        x = a [t5]
	L10:	t6 = i * 8
	        t7 = j * 8
	        t8 = a [t7]
	        a[t6] = t8
	L11:	t9 = j * 8
	        a[t9] = x
	        goto L1
	L2:
	
