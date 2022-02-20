#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* CONSTANTS */
#define TENANTS_AMOUNT 10
#define REALTORS_AMOUNT 5

struct args {
    int counter;
    int tenant;
};

int counter = (intptr_t) 0;

/* threads */
int execute_rent = 1; 
int execute_return = 1;
int execute_add = 1;
int execute_remove = 1;

/*available real states*/
int available_real_state[15] = {15, 33, 58, 99, 105, 203, 178};

/*unavailable real states*/
int unavailable_real_state[15];

/*functions*/
void *rentRealState(void *arg);
void *returnRealState(void *arg);
void *makeAvailableRealState(void *arg);

int main() {
    pthread_t tenants[TENANTS_AMOUNT];
    pthread_t realtors[REALTORS_AMOUNT];

    srand(time(NULL));

    int count = 0;
    while (count++ < 50) {
        if (execute_rent == 0) {
            sleep(4);
        }

        execute_rent = rand()%2;

        if (execute_rent == 1) {
            execute_rent = 0;

            int countRent = (intptr_t) 0;
            int realStateId = (intptr_t) 0;

            while (realStateId == 0 && countRent++ != 15) {
                realStateId = available_real_state[countRent];
            }

            if (available_real_state[countRent] != 0) {
                struct args *rent = (struct args *)malloc(sizeof(struct args));
                int tenantId = (intptr_t) rand()%10;
                rent->counter = countRent;
                rent->tenant = tenantId;
                pthread_create(&tenants[tenantId], NULL, rentRealState, (void *) rent);
            }
        }

        if (execute_return == 1) { o
            int execute_return = 0;
            int realStateId = (intptr_t) rand()%15; 

            if (unavailable_real_state[realStateId] != 0){ 

                struct args *devolution = (struct args *)malloc(sizeof(struct args));
                int tenantId = rand()%10;
                int realtorRent = rand()%5;
                devolution->counter = realStateId;
                devolution->tenant = tenantId;

                struct args *realtorReturn = (struct args *)malloc(sizeof(struct args));
                realtorReturn->counter = realStateId;
                realtorReturn->tenant = realtorRent;

                pthread_create(&tenants[tenantId], NULL, returnRealState, (void *) devolution);
                pthread_create(&realtors[realtorRent], NULL, makeAvailableRealState, (void *) realtorReturn);
            }
        }
    }

    count = 0;
    while(count++ < TENANTS_AMOUNT) {
        pthread_join(tenants[count], NULL);
    }

    printf("Finaliza o processo\n\n");
    exit(EXIT_SUCCESS);
}

void *rentRealState(void *arg) {
    int count = 0;

    int realStateIndex = ((struct args*) arg)->counter;
    int tenantId = ((struct args*) arg)->tenant;

    if (available_real_state[realStateIndex] != 0) {
        while(count++ < 16) {
            if (unavailable_real_state[count] == 0) {
                unavailable_real_state[count] = available_real_state[realStateIndex];
                printf("Inquilino %d aluga imovel %d\n", tenantId, available_real_state[realStateIndex]);
                available_real_state[realStateIndex] = 0;
                break; 
            }
        }
    }

    execute_rent = 1;
    pthread_exit(NULL);
    return NULL;
}

void *returnRealState(void *arg){
  int realState = ((struct args*) arg)->counter;
  int tenant = ((struct args*) arg)->tenant;

  printf("Inquilino %d devolveu o imovel: %d\n", tenant, unavailable_real_state[realState]);
  pthread_exit(NULL);
  return NULL;
}

void *makeAvailableRealState(void *arg){
  int count = 0;
  int realState = ((struct args*) arg)->counter;
  int realtor = ((struct args*) arg)->tenant;


  while(count++ < 16){
    if(available_real_state[count] == 0) { 
      available_real_state[count] = unavailable_real_state[realState];
      printf("Corretor %d disponibiliza o imovel: %d\n", realtor, available_real_state[count]);
      unavailable_real_state[realState] = 0;
      break;
    }
  }

  pthread_exit(NULL);
  return NULL;
}

