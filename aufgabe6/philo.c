#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Gabeln realisiert durch semaphoren, damit jede Gabel immer nur von einem Prozess verwendet werden kann
// Semaphore p wird benötigt um die Verklemmung zu verhindern
sem_t f1,f2,f3,f4,f5,p;
// exit Bedingung
int t = 0;

// Thread Argumente
struct arg_struct{
  int thread;
};

// Darf immer nur von einem Thread verwendet werden(Semaphore p)
// Alle Gabeln müssen auf einmal bereit gestellt werden. => Verhindert Verklemmung siehe Vorlesung
void getFork(int i){
// Pro Thread ein case => bei 5 Threads einfachste realisierung
  switch (i) {
    case 1: sem_wait(&f1);
            sem_wait(&f2);
	    sem_post(&p);
            sleep(2);
            printf("Thread %d finished \n", i);
	    t++;
            sem_post(&f1);
            sem_post(&f2);
            break;
    case 2: sem_wait(&f2);
            sem_wait(&f3);
	    sem_post(&p);
            sleep(2);
            printf("Thread %d finished \n", i );
	    t++;
            sem_post(&f2);
            sem_post(&f3);
            break;
    case 3: sem_wait(&f3);
            sem_wait(&f4);
	    sem_post(&p);
            sleep(2);
            printf("Thread %d finished \n", i );
	    t++;
            sem_post(&f3);
            sem_post(&f4);
            break;
    case 4: sem_wait(&f4);
            sem_wait(&f5);
	    sem_post(&p);
            sleep(2);
            printf("Thread %d finished \n", i );
	    t++;
            sem_post(&f4);
            sem_post(&f5);
            break;
    case 5: sem_wait(&f5);
            sem_wait(&f1);
	    sem_post(&p);
            sleep(2);
            printf("Thread %d finished \n", i );
	    t++;
            sem_post(&f5);
            sem_post(&f1);
            break;
  }


}

// Zentrale Instanz, die den Zugriff auf die Gablen verwaltet durch die Semaphore p
void *forkDisposer(void *i){
  // Typecast des void arguments zurück in ein int
  struct arg_struct *args = (struct arg_struct *)i;
  int thread = args->thread;
  sem_wait(&p);
  getFork(thread);
  return 0;

}



int main(){

  //Initialisierung der Threads + Argumente
  pthread_t p1, p2, p3, p4 ,p5;
  struct arg_struct arg1, arg2, arg3, arg4, arg5;
  // Initialisierung der oben definierten Semaphoren
  sem_init(&f1, 0, 1);
  sem_init(&f2, 0, 1);
  sem_init(&f3, 0, 1);
  sem_init(&f4, 0, 1);
  sem_init(&f5, 0, 1);
  sem_init(&p, 0, 1);

  // Zuweisung der Thread-ID für switch-case damit jeder die Gabeln zu seiner Linken und Rechten verwendet
  arg1.thread = 1;
  arg2.thread = 2;
  arg3.thread = 3;
  arg4.thread = 4;
  arg5.thread = 5;

  // Starten der Threads
  pthread_create(&p1, NULL, forkDisposer, &arg1);
  pthread_create(&p2, NULL, forkDisposer, &arg2);
  pthread_create(&p3, NULL, forkDisposer, &arg3);
  pthread_create(&p4, NULL, forkDisposer, &arg4);
  pthread_create(&p5, NULL, forkDisposer, &arg5);

  // warten bis threads beendet sind
  while(t<5){}
  // Terminierung
  return 0;
}
