#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "imovel.h"


/* CONSTANTS OF TENANTS AND REALTORS*/
#define TENANTS_AMOUNT 10
#define REALTORS_AMOUNT 5

struct args {
    int counter;
    int tenant;
};

int counter = (intptr_t) 0;
int code = 0;
const int CHAR_SIZE = 200;

/* threads */
int execute_rent = 1; 
int execute_return = 1;
int execute_add = 1;
int execute_remove = 1;

/*available real states*/
int available_real_state[15] = {15, 33, 58, 99, 105, 203, 178};

/*unavailable real states*/
int unavailable_real_state[15];

char address[][200] = {"Rua Ilha Três Irmãs, 12", "Avenida Osmar Cunha, 789", "Rodovia Cardoso, 700", "Rua Marco Antonio, 987", "Avenida das Flores, 500"};
char district[][200] = {"Ingleses", "Barra da Lagoa", "Carianos", "Saco dos Limoes", "Coqueiros"};

imovel_t rs; 

/*functions*/
void *addRealState(void *arg);
void *deleteRealState(void *arg);
void *rentRealState(void *arg);
void *returnRealState(void *arg);
void *makeAvailableRealState(void *arg);
imovel_t RandomRealState();

int main() {
    pthread_t tenants[TENANTS_AMOUNT];
    pthread_t realtors[REALTORS_AMOUNT];

    srand(time(NULL));

    printf("Imóveis criados:\n");

    for(int i=0;i<10;i++) {
        rs = RandomRealState();
        available_real_state[i] = rs.codigo;
        printf("Código: %d, Rua: %s, Bairro: %s, Preço: %d\n", rs.codigo, rs.endereco, rs.bairro, rs.preco);
    }

    int count = 0;

    while (count++ < 50) {
        if (execute_rent == 0) {
            sleep(4);
        }

        // execute_add = rand()%8;
        execute_remove = rand()%8;
        execute_return = rand()%4;
        execute_rent = rand()%2;

        if (execute_rent == 1) {
            execute_rent = 0;

            int countRent = (intptr_t) 0;
            int realStateId = (intptr_t) 0;

            while (realStateId == 0 && countRent++ != 10) {
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

        // if (execute_add == 1 ){
        //     int realStateIndex = (intptr_t) rand()%20;

        //     if (available_real_state[realStateIndex] != 0){

        //         struct args *realtor = (struct args *)malloc(sizeof(struct args));

        //         int x = rand()%5;

        //         realtor->counter = realStateIndex;
        //         realtor->tenant = x;

        //         pthread_create(&realtors[x], NULL, addRealState, (void *) realtor);
        //     }
        // }

        if (execute_remove == 1 ){
            int realStateIndex = (intptr_t) rand()%20;

            if (available_real_state[realStateIndex] != 0){

                struct args *realtor = (struct args *)malloc(sizeof(struct args));

                int x = rand()%5;

                realtor->counter = realStateIndex;
                realtor->tenant = x;

                pthread_create(&realtors[x], NULL, deleteRealState, (void *) realtor);
            }
        }

        if (execute_return == 1) { 
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

imovel_t RandomRealState(){
  code = code + rand()%50;
  imovel_t rs;
  rs.codigo = code;
  rs.preco = 250 + rand()%300;
  strcpy(rs.endereco,address[rand()%(sizeof(address)/CHAR_SIZE)]);
  strcpy(rs.bairro,district[rand()%(sizeof(district)/CHAR_SIZE)]);
  return rs;
}

// void *addRealState(void *arg){

//   int tenantId = ((struct args*) arg)->tenant;
//   int realStateIndex = ((struct args*) arg)->counter;

//   printf("Corretor %d adicionou o imóvel: %d\n", tenantId, available_real_state[realStateIndex]);


//   printf("Corretor %d adicionou o imóvel: %d\n", tenantId, available_real_state[realStateIndex]);
//   available_real_state[realStateIndex] = 0;
//   pthread_exit(NULL);
//   return NULL;
// }

void *deleteRealState(void *arg){
  int tenantId = ((struct args*) arg)->tenant;
  int realStateIndex = ((struct args*) arg)->counter;

  printf("Corretor %d removeu o imóvel: %d\n", tenantId, available_real_state[realStateIndex]);
  available_real_state[realStateIndex] = 0;
  pthread_exit(NULL);
  return NULL;
}

void *rentRealState(void *arg) {
    int count = 0;

    int tenantId = ((struct args*) arg)->tenant;
    int realStateIndex = ((struct args*) arg)->counter;

    if (available_real_state[realStateIndex] != 0) {
        while(count++ < 16) {
            if (unavailable_real_state[count] == 0) {
                unavailable_real_state[count] = available_real_state[realStateIndex];
                printf("Inquilino %d alugou o imóvel %d\n", tenantId, available_real_state[realStateIndex]);
                available_real_state[realStateIndex] = 0;
                break; 
            }
        }
    }

    execute_rent = 1;
    pthread_exit(NULL);
    return NULL;
}


void *makeAvailableRealState(void *arg){
  int count = 0;
  int realtor = ((struct args*) arg)->tenant;
  int realState = ((struct args*) arg)->counter;


  while(count++ < 16){
    if(available_real_state[count] == 0) { 
      available_real_state[count] = unavailable_real_state[realState];
      printf("Corretor %d disponibiliza o imóvel: %d\n", realtor, available_real_state[count]);
      unavailable_real_state[realState] = 0;
      break;
    }
  }

  pthread_exit(NULL);
  return NULL;
}

void *returnRealState(void *arg){
  int tenant = ((struct args*) arg)->tenant;
  int realState = ((struct args*) arg)->counter;

  printf("Inquilino %d devolveu o imóvel: %d\n", tenant, unavailable_real_state[realState]);
  pthread_exit(NULL);
  return NULL;
}
