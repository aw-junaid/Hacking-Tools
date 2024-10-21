

int code(a)
char *a;
{
  while(*a !='\0') {
   *a = ~(*a);
   a++;
  }
  return(1);
}
