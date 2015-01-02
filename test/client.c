#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <zmq.h>


int main()
{
  printf("hello, world\n");
  
  void *ctx = zmq_ctx_new();
  void *sock_req;
  if ((sock_req =zmq_socket(ctx, ZMQ_REQ)) == NULL) {
    perror("zmq_socket@main");
    exit(1);
  }
  if (zmq_connect(sock_req, "tcp://localhost:5557") == -1) {
    perror("zmq_connect@main");
    exit(1);
  }

  char addr[128] = "tcp://*:5999";
  void *sock_rep;
  if ((sock_rep = zmq_socket(ctx, ZMQ_REP)) == NULL) {
    perror("zmq_socket@main");
    exit(1);
  }
  if (zmq_bind(sock_rep, addr) == -1) {
    perror("zmq_bind@main");
    exit(1);
  }

  char sstr[1024];
  char rstr[1024];

  unsigned long long counter = 0;
  unsigned long long ntrials = 2;

  struct timeval tv0;
  struct timeval tv1;

  gettimeofday(&tv0, NULL);
  while (1) {
    if (counter > ntrials)
      break;

    if (counter % 2) {
      sprintf(sstr, 
              "{"
              " \"id\":%Ld, "
              " \"src\":\"%s\", "
              " \"tag\":\"%s\", "
              " \"arg\":"
              "{ \"name\": \"foo\", \"funarg\" : 123 } }",
              counter++, addr, 
              "FUNC_CALL");
    }
    else {
      sprintf(sstr, 
              "{"
              " \"id\":%Ld, "
              " \"src\":\"%s\", "
              " \"tag\":\"%s\", "
              " \"arg\":"
              "{ \"name\": \"gpioPut\", \"funarg\" : [10 1] } }",
              counter++, addr, 
              "FUNC_CALL");
      // sprintf(sstr, "DEVADD dev0 127.0.0.1"); 
      // counter++;
      /* sprintf(sstr, "DEVADD dev0 127.0.0.1")
              "{"
              " \"id\":%Ld, "
              " \"src\":\"%s\", "
              " \"tag\":\"%s\", "
              " \"arg\":"
              "{ \"name\": \"foo\", \"funarg\" : 123 } }",
              counter++, addr, 
              "FUNC_CALL", counter);*/
      /*sprintf(sstr, 
              "{"
              " \"id\":%Ld, "
              " \"src\":\"%s\", "
              " \"tag\":\"%s\", "
              " \"arg\":"
              "{ \"name\": \"gpioPut\", \"funarg\" : [ 123, 2 ] } }",
              counter++, addr, 
              "FUNC_CALL", counter);*/
    }

    // send request
    zmq_msg_t smsg;
    zmq_msg_init_size(&smsg, strlen(sstr));
    /*
      sprintf(sstr, 
            "{"
            " \"id\":%Ld, "
            " \"src\":\"%s\", "
            " \"tag\":\"%s\", "
            " \"arg\":"
            "{ \"name\": \"temp_change\", \"value\" : %Ld } }",
            counter++, addr, 
            "EVENT_OCCUR", counter);
    */

    memcpy(zmq_msg_data(&smsg), sstr, strlen(sstr));
    int sz = zmq_msg_send(&smsg, sock_req, 0);
    zmq_msg_close(&smsg);

    // fprintf(stdout, "message sent: %s\n", sstr);

    // check reply
    zmq_msg_t rmsg;
    zmq_msg_init(&rmsg);
    int rsz;
    if ((rsz = zmq_msg_recv(&rmsg, sock_req, 0)) != -1) {
      char *rstr = malloc(rsz + 1);
      memcpy(rstr, (char *) zmq_msg_data(&rmsg), rsz);
      zmq_msg_close(&rmsg);
      rstr[rsz] = '\0';
      // fprintf(stdout, "message recv: %s\n", rstr);
      free(rstr);
    }
    else {
      perror("zmq_msg_recv");
      exit(1);
    }
  }

  gettimeofday(&tv1, NULL);

  long long int diff = (tv1.tv_usec + tv1.tv_sec * 1000000) -
    (tv0.tv_usec + tv0.tv_sec * 1000000);
  float nsec = ((float) diff) / 1000000;
  fprintf(stdout, "Elapsed time for %Ld messages: %.1f sec (%.1f m/s)\n", 
          ntrials, nsec,  ((float) ntrials)/nsec);

  zmq_close(sock_req);
  zmq_close(sock_rep);
}
