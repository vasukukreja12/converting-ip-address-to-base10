
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <locale.h>
#include <regex.h>

#define PACKAGE "ip2b10"
#define VERSION "0.0.2"
#define URL     "WWW"
#define IPEXPR  "([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})"

/* status epilepticus, print help and exit with exval */
void print_help(int exval);
/* print version and exit with exval */
void print_version(int exval);

int main(int argc, char *argv[]) {
 regex_t re;
 int opt = 0;
 int print_flag = 0;
 long unsigned int result = 0;
 int var1 = 0, var2 = 0;
 int var3 = 0, var4 = 0;

 /* any options given ? */
 if(argc == 1)
  print_help(1);

 /* option parser */
 while((opt = getopt(argc, argv, "hvxa")) != -1) {
  switch(opt) {
   case 'h': /* print this help and exit */
    print_help(0);
   case 'v': /* print program version and exit */
    print_version(0);
   case 'x': /* output in Hex.. */
    print_flag = 1;
    break;
   case 'a': /* output some additional info... */
    print_flag = 2;
    break;
   case '?': /* no such option */
    fprintf(stderr, "%s: Error - No such option: `%c'\n\n", PACKAGE, optopt);
    print_help(1);
  }
 }

 /* enough options left ? */
 if((argc - optind) == 0)
  print_help(1);

 /* compile regular expression */
 if(regcomp(&re, IPEXPR, REG_EXTENDED) != 0) {
  fprintf(stderr, "%s: Error - compiling regular expression..\n", 
    PACKAGE);
  return 1;
 }

 /* while parsing remaining opts.. */
 for(; optind < argc; optind++) {
  /* very basic test of ip4 dotted quad address format */
  if(regexec(&re, argv[optind], 0, NULL, 0) != 0) {
   fprintf(stderr, "%s: Error - `%s'; not a ip4 dotted quad..\n", 
     PACKAGE, argv[optind]);
   continue;
  }

  /* read address */
  sscanf(argv[optind], "%d.%d.%d.%d", &var1, &var2, &var3, &var4);
  /* convert address to base 10 */
  result = (var1 << 24) + (var2 << 16) + (var3 << 8) + var4;

  /* output the results */
  if(print_flag == 0)
   printf("%lu\n", result);
  else if(print_flag == 1)
   printf("%0lx\n", result);
  else {
   printf("%%\n");
   printf("ip addr : %s\n", argv[optind]);
   printf("formula : %d * (256^3) + %d * (256^2) + %d * 256 + %d\n", 
     var1, var2, var3, var4);
   printf("base10  : %lu\n", result);
   printf("hex     : %0lx\n", result);
  }
 }

 return 0;
}

/* status epilepticus, print help and exit with exval */
void print_help(int exval) {
 printf("%s,%s convert an IP address to its base10 form\n", PACKAGE, VERSION);
 printf("Usage: %s [-h] [-v] [-x] [-a] IP IP..\n\n", PACKAGE);

 printf(" -h        print this help and exit\n");
 printf(" -v        print version and exit\n");
 printf(" -x        print address in Hexadecimal\n");
 printf(" -a        print additional info\n\n");

 printf(" Please note, this is valid for IP version 4 addresses only!\n");
 exit(exval);
}

/* print version and exit with exval */
void print_version(int exval) {
 exit(exval);
}
