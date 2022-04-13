#include "types.h"
#include "stat.h"
#include "user.h"
#include "mmu.h"

int
main(int argc, char *argv[])
{
      // Create value to be protected
      char *n = sbrk(0);
      sbrk(PGSIZE);
      printf(1, "\nPage size: %d\n", PGSIZE);

      // Initialise value and set it to unprotected by default
      *n = 25;
      printf(1, "\nThe initial value is %d. Set to unprotected by default \n", *n);
      munprotect(n, 1);

      printf(1, "\n Forked child values:\n");

      // Create new children processes by forking with parent process running
      int child = fork();
      printf(1, "%d\n", child);

      // If the child's value is 0
      if (child <= 0) {
            munprotect(n, 1);  
            printf(1, "\n Read only protection removed from child with value %d\n", child);


            // Test protection removal by changing the value (should run)
            printf(1, "Changing value after removing read only mode...\n");
            *n = 20;
            printf(1, "New value: %d\n", *n);
            exit();
      } 
      // If the child's value is greater than 0
      else if (child > 0) {
            wait();
            // Add read only protection
            mprotect(n, 1);
            printf(1, "\n Read only protection added to child with value %d\n", child);

            // Test protection by changing the value (should throw an error)
            printf(1, "Changing value while in read only mode...\n");
            *n = 10;
            exit(); 
      }
      exit();
}