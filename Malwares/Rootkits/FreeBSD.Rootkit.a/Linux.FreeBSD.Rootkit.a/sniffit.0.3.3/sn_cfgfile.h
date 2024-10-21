void clear_list_buffer (struct cfg_file_contense *help)
{
help->host[0]=0;
help->priority=0;
help->port=0;
help->wildcard=0;
}

struct cfg_file_contense *adjust_select_from_list (void)
{
Priority++;
select_from_length++; 

select_from_list=(struct cfg_file_contense *)realloc(select_from_list,
			select_from_length*sizeof(struct cfg_file_contense));
if(select_from_list==NULL)
	{printf("Sniffit hartattack... out of memory!\n"); exit(1);}
clear_list_buffer(&(select_from_list[select_from_length-1])); 
return &(select_from_list[select_from_length-1]); 
}

struct cfg_file_contense *adjust_select_to_list (void)
{
Priority++;
select_to_length++; 
select_to_list=(struct cfg_file_contense *)realloc(select_to_list,
			select_to_length*sizeof(struct cfg_file_contense));
if(select_to_list==NULL)
	{printf("Sniffit hartattack... out of memory!\n"); exit(1);}
clear_list_buffer(&(select_to_list[select_to_length-1]));
return &(select_to_list[select_to_length-1]);
}

struct cfg_file_contense *adjust_deselect_from_list (void)
{
Priority++;
deselect_from_length++; 
deselect_from_list=(struct cfg_file_contense *)realloc(deselect_from_list,
			deselect_from_length*sizeof(struct cfg_file_contense));
if(deselect_from_list==NULL)
	{printf("Sniffit hartattack... out of memory!\n"); exit(1);}
clear_list_buffer(&(deselect_from_list[deselect_from_length-1]));
return (&(deselect_from_list[deselect_from_length-1]));
}

struct cfg_file_contense *adjust_deselect_to_list (void)
{
Priority++;
deselect_to_length++; 
deselect_to_list=(struct cfg_file_contense *)realloc(deselect_to_list,
			deselect_to_length*sizeof(struct cfg_file_contense));
if(deselect_to_list==NULL)
	{printf("Sniffit hartattack... out of memory!\n"); exit(1);}
clear_list_buffer(&(deselect_to_list[deselect_to_length-1]));
return &(deselect_to_list[deselect_to_length-1]);
}

char *clean_string (char *string)
{
char help[20];
int i, j;

j=0;
for(i=0;i<strlen(string);i++)
	{
	if( (isalnum(string[i]))||(string[i]=='.') )
		{
		help[j]=string[i];
		help[j+1]=0;
		}
	j++;
	}
strcpy(string, help);
return string;
}

char *clean_filename (char *string)
{
char help[20];
int i, j;

j=0;
for(i=0;i<strlen(string);i++)
	{
	if( !(iscntrl(string[i])) && !(isspace(string[i])) )
		{
		help[j]=string[i];
		help[j+1]=0;
		}
	j++;
	}
strcpy(string, help);
return string;
}

void make_nr_dot (char *host)
{
unsigned long hostnr;
unsigned char *digit;
char help[255];

strcpy(help, host);
clean_string(help);
hostnr=getaddrbyname(help);
digit=(char *)&hostnr;
sprintf(dot_notation,"%u.%u.%u.%u",digit[0],digit[1],digit[2],digit[3]);
}

void interprete_line (char *line)
{
struct cfg_file_contense *help, *helpp; 
char *field;
size_t i;

field=strtok(line," ");
if(field!=NULL)
	{
	strlower(field);
	if(strcmp(strlower(field),"logfile")==0)
	  {
	  field=clean_filename(strtok(NULL," "));
     	  if(field != NULL) strcpy(Logfile,field);
	  }
	if(strcmp(strlower(field),"select")==0)
	  {
	  field=strtok(NULL," ");
          if(strcmp(strlower(field),"from")==0)
	    {
            field=strtok(NULL," ");
            if(strcmp(strlower(field),"host")==0)  /* select from host */
	      {
	      help=adjust_select_from_list();
              make_nr_dot(strtok(NULL," "));
              strcpy(help->host,dot_notation);
              if( (field=strtok(NULL," "))!=NULL) 
			help->port=atoi(field);
	      help->priority=Priority;
              return;
 	      }
            if(strcmp(strlower(field),"port")==0)  /* select from port */
	      {                           
	      help=adjust_select_from_list();
	      help->port=atoi( strtok(NULL," ") ); 
	      help->priority=Priority;
	      return;
	      }
            if(strcmp(strlower(field),"mhosts")==0)  /* select from mhosts */
	      {
	      help=adjust_select_from_list();
	      strcpy(help->host, clean_string(strtok(NULL," ")));
	      if( (field=strtok(NULL," "))!=NULL)
			help->port=atoi(field);
	      help->priority=Priority;
	      help->wildcard=1;
 	      return;
	      }
 	    }
          if(strcmp(strlower(field),"to")==0)
	    {
            field=strtok(NULL," ");
            if(strcmp(strlower(field),"host")==0)  /* select to host */
	      {
	      help=adjust_select_to_list();
              make_nr_dot(strtok(NULL," "));
	      strcpy(help->host,dot_notation);
	      if( (field=strtok(NULL," "))!=NULL)
			help->port=atoi(field);
	      help->priority=Priority;
              return;
 	      }
            if(strcmp(strlower(field),"port")==0)  /* select to port */
	      {                           
	      help=adjust_select_to_list();
	      help->port=atoi( strtok(NULL," ") ); 
	      help->priority=Priority;
              return;
 	      }
            if(strcmp(strlower(field),"mhosts")==0)  /* select to mhosts */
	      {                           
	      help=adjust_select_to_list();
	      strcpy(help->host, clean_string(strtok(NULL," ")));
	      if( (field=strtok(NULL," "))!=NULL)
			help->port=atoi(field);
	      help->priority=Priority;
	      help->wildcard=1;
              return;
 	      }
 	    }
          if(strcmp(strlower(field),"both")==0)
	    {
            field=strtok(NULL," ");
            if(strcmp(strlower(field),"host")==0)  /* select both host */
	      {
	      help=adjust_select_from_list();
	      helpp=adjust_select_to_list();
              make_nr_dot(strtok(NULL," "));
	      strcpy(help->host,dot_notation);
	      strcpy(helpp->host,dot_notation);
	      if( (field=strtok(NULL," "))!=NULL)
			{help->port=atoi(field); helpp->port=atoi(field);}
	      help->priority=Priority;
 	      helpp->priority=Priority;
              return;
	      }
            if(strcmp(strlower(field),"port")==0)  /* select both port */
	      {                           
	      help=adjust_select_from_list();
	      helpp=adjust_select_to_list();
	      field=strtok(NULL," "); 
	      help->port=atoi(field); 
	      helpp->port=atoi(field); 
	      help->priority=Priority;
	      helpp->priority=Priority;
              return;
 	      }
            if(strcmp(strlower(field),"mhosts")==0)  /* select both mhosts */
	      {                           
	      help=adjust_select_from_list();
	      helpp=adjust_select_to_list();
	      field=clean_string(strtok(NULL," "));
	      strcpy(help->host, field);
	      strcpy(helpp->host, field);
	      if( (field=strtok(NULL," "))!=NULL)
			{help->port=atoi(field); helpp->port=atoi(field);}
	      help->priority=Priority;
	      helpp->priority=Priority;
 	      help->wildcard=1;
	      helpp->wildcard=1;
              return;
	      }
 	    }
	  }
	if(strcmp(strlower(field),"deselect")==0)
	  {
	  field=strtok(NULL," ");
          if(strcmp(strlower(field),"from")==0)
	    {
            field=strtok(NULL," ");
            if(strcmp(strlower(field),"host")==0)  /* select from host */
	      {
	      help=adjust_deselect_from_list();
              make_nr_dot(strtok(NULL," "));
	      strcpy(help->host,dot_notation);
	      if( (field=strtok(NULL," "))!=NULL)
			help->port=atoi(field);
	      help->priority=Priority;
              return;
 	      }
            if(strcmp(strlower(field),"port")==0)  /* select from port */
	      {                           
	      help=adjust_deselect_from_list();
	      help->port=atoi(strtok(NULL," ")); 
	      help->priority=Priority;
              return;
 	      }
            if(strcmp(strlower(field),"mhosts")==0)  /* select from mhosts */
	      {                           
	      help=adjust_deselect_from_list();
	      strcpy(help->host, clean_string(strtok(NULL," ")));
	      if( (field=strtok(NULL," "))!=NULL)
			help->port=atoi(field);
	      help->priority=Priority;
 	      help->wildcard=1;
              return;
 	      }
 	    }
          if(strcmp(strlower(field),"to")==0)
	    {
            field=strtok(NULL," ");
            if(strcmp(strlower(field),"host")==0)  /* deselect to host */
	      {
	      help=adjust_deselect_to_list();
              make_nr_dot(strtok(NULL," "));
	      strcpy(help->host,dot_notation);
	      if( (field=strtok(NULL," "))!=NULL)
			help->port=atoi(field);
	      help->priority=Priority;
              return;
 	      }
            if(strcmp(strlower(field),"port")==0)  /* deselect to port */
	      {                           
	      help=adjust_deselect_to_list();
	      help->port=atoi(strtok(NULL," ")); 
	      help->priority=Priority;
              return;
 	      }
            if(strcmp(strlower(field),"mhosts")==0)  /* deselect to mhosts */
	      {                           
	      help=adjust_deselect_to_list();
	      strcpy(help->host, clean_string(strtok(NULL," ")));
	      if( (field=strtok(NULL," "))!=NULL)
			help->port=atoi(field);
	      help->priority=Priority;
 	      help->wildcard=1;
              return;
	      }
 	    }
          if(strcmp(strlower(field),"both")==0)
	    {
            field=strtok(NULL," ");
            if(strcmp(strlower(field),"host")==0)  /* deselect both host */
	      {
	      help=adjust_deselect_from_list();
	      helpp=adjust_deselect_to_list();
              make_nr_dot(strtok(NULL," "));
	      strcpy(help->host,dot_notation);
	      strcpy(helpp->host,dot_notation);
	      if( (field=strtok(NULL," "))!=NULL)
			{help->port=atoi(field); helpp->port=atoi(field);}
	      help->priority=Priority;
	      helpp->priority=Priority;
              return;
 	      }
            if(strcmp(strlower(field),"port")==0)  /* deselect both port */
	      {                           
	      help=adjust_deselect_from_list();
	      helpp=adjust_deselect_to_list();
	      field=strtok(NULL," "); 
	      help->port=atoi(field); 
	      helpp->port=atoi(field); 
	      help->priority=Priority;
	      helpp->priority=Priority;
              return;
	      }
            if(strcmp(strlower(field),"mhosts")==0)  /* deselect both mhosts */
	      {                           
	      help=adjust_deselect_from_list();
	      helpp=adjust_deselect_to_list();
	      field=clean_string(strtok(NULL," "));
	      strcpy(help->host, field);
	      strcpy(helpp->host, field);
	      if( (field=strtok(NULL," "))!=NULL)
			{help->port=atoi(field); helpp->port=atoi(field);}
	      help->priority=Priority;
	      helpp->priority=Priority;
 	      help->wildcard=1;
 	      helpp->wildcard=1;
              return;
 	      }
 	    }
 	  }
	}
}

void read_cfg_file (char *file)
{
FILE *cfgfile;
size_t line_length;
char lineptr[260];

select_from_list=NULL; select_to_list=NULL;
deselect_from_list=NULL; deselect_to_list=NULL;
Priority=0;

if((cfgfile = fopen(file,"r"))==NULL)
	{printf("Couldn't find config file... giving up.\n"); exit(1);}
while(feof(cfgfile)==0)
	{
	fgets(lineptr,259,cfgfile);
        if(feof(cfgfile)==0)
	   {
	   interprete_line(lineptr);}
	}
fclose(cfgfile);
};

