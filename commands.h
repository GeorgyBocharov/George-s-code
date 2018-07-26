COM (mul, 1, 
{if (Mul(my_stack) == ERR)
{
	printf("Error of mul\n");
	exit(EXIT_FAILURE);
}
break;})

COM (pop, 2, 
{
	if (Pop(my_stack, &buf) == ERR)
	{
		printf("Error of pop\n");
		exit(EXIT_FAILURE);
	}
break;})

COM (add, 3,
{if (Add(my_stack) == ERR)
{
	printf("Error of add\n");
	exit(EXIT_FAILURE);
}
break;})


COM (sub, 4,
{if (Sub(my_stack) == ERR)
{
	printf("Error of sub\n");
	exit(EXIT_FAILURE);
}
break;})

COM (out, 5,
{if (Out(my_stack) == ERR)
{
	printf("Error of out\n");
	exit(EXIT_FAILURE);
}
break;})

COM (end, 6,{ count = height; break;})

COM (ret, 7,
{StackElem_t help = (StackElem_t) count;	
	if (Pop(adress_stack, &help) == ERR)
{
	printf("Error of add\n");
	exit(EXIT_FAILURE);
}
count = (int) help;
count--;
break;})

COM (div, 8, 
{ if (Div(my_stack) == ERR)
{
	printf("Error of div\n");
	exit(EXIT_FAILURE);
}
break;})

COM (in, 9, 
{
StackElem_t value;
if (EOF == scanf("%lg", &value))
{
	printf("error of scanf\n");
	exit(EXIT_FAILURE);
}
if (Push (my_stack, value) == ERR)
{
	printf("error of pushing in command in, line: %d", count);
	exit(EXIT_FAILURE);
}
break;})

COM (jmp, 10,
{count = (int) MainArr[count + 1] - 1;
break;})

COM (je, 11, 
count = Je(my_stack, (int) MainArr[count + 1], count);
count++;
break;)

COM (jne, 12,
{count = Jne(my_stack, (int) MainArr[count + 1], count);
count++;
break;})

COM (ja, 13,
{count = Ja(my_stack, (int) MainArr[count + 1], count);
count++;
break;})

COM (jb, 14,
{count = Jb(my_stack, (int) MainArr[count + 1], count);
count++;
break;})

COM (jae, 15,
{count = Jae(my_stack, (int) MainArr[count + 1], count);
count++;
break;})

COM (jbe, 16,
{count = Jbe(my_stack, (int) MainArr[count + 1], count);
count++;
break;})

COM (call, 17,
{
if (Push(adress_stack, (count + 2)) == ERR)
{
	printf("Error of add\n");
	exit(EXIT_FAILURE);
}
count = (int) MainArr[count + 1] - 1;
break;})

COM (pop_reg, 18,
{	int i;	
	for (i = 1; i <= 4; i++)
	{
		if (fabs(MainArr[count + 1] - i) < EPS)
		{
			if (Pop_reg(my_stack, &(my_struct->registr[i - 1])) == ERR)
			{
				printf("Error of registr pop\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	count++;
break;})		

COM (push, 20,
{if (fabs(MainArr[count + 1]) < EPS)
{
	if (Push (my_stack, MainArr[count + 2]) == ERR)
	{
		printf("error of pushing, line: %d", count);
		exit(EXIT_FAILURE);
	}
}
else if (fabs(MainArr[count + 1] - 1) < EPS)
{
	int i;	
	for (i = 1; i <= 4; i++)
	{
		if (fabs(MainArr[count + 2] -  i) < EPS)
		{
			if (Push(my_stack, my_struct->registr[i - 1]) == ERR)
			{
				printf("Error of registr push\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}
else 
{
	printf("error of push");
	exit(EXIT_FAILURE);
}
count += 2;
break;})


