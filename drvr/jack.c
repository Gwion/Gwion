#include <stdlib.h>
#include <jack/jack.h>

#include <unistd.h> // for sleep

#include "vm.h"
#include "bbq.h"
#include "driver.h"
jack_port_t** iport;
jack_port_t** oport;
jack_client_t* client;

void jack_wakeup()
{
//	jack_client_close(client);
  jack_deactivate(client);
}


static void gwion_shutdown (void *arg)
{
  VM *vm = (VM *)arg;
  vm->is_running = 0;
}

static int gwion_cb(jack_nframes_t nframes, void *arg)
{
  int frame, chan;
  VM* vm  = (VM*)arg;
  jack_default_audio_sample_t  * in[vm->bbq->n_in];
  jack_default_audio_sample_t  * out[vm->bbq->sp->nchan];
  for(chan = 0; chan < vm->bbq->n_in; chan++)
    in[chan] = jack_port_get_buffer (iport[chan], nframes);
  for(chan = 0; chan < vm->bbq->sp->nchan; chan++)
    out[chan] = jack_port_get_buffer (oport[chan], nframes);
  for(frame = 0; frame < nframes; frame++) {
    for(chan = 0; chan < vm->bbq->n_in; chan++)
      vm->bbq->in[chan] = in[chan][frame];
    vm_run(vm);
    for(chan = 0; chan < vm->bbq->sp->nchan; chan++)
      out[chan][frame] = vm->bbq->sp->out[chan];
    vm->bbq->sp->pos++;
  }
  return 0;
}

m_bool jack_ini(VM* vm, DriverInfo* di)
{
  m_str  client_name = "Gwion";
  const m_str  server_name = NULL;
  m_uint chan;
  jack_options_t options = JackNullOption;
  jack_status_t status;
  iport = malloc(sizeof(jack_port_t *) * di->in);
  oport = malloc(sizeof(jack_port_t *) * di->out);
//	client = malloc(sizeof(jack_client_t *));
  client = jack_client_open (client_name, options, &status, server_name);
  if(!client) {
    fprintf (stderr, "jack_client_open() failed, "
             "status = 0x%2.0x\n", status);
    if (status & JackServerFailed)
      fprintf (stderr, "Unable to connect to JACK server\n");
    return -1;
  }
  if (status & JackNameNotUnique) {
    client_name = jack_get_client_name(client);
    fprintf (stderr, "unique name `%s' assigned\n", client_name);
  }
  jack_set_process_callback (client, gwion_cb, vm);
  jack_on_shutdown (client, gwion_shutdown, vm);

  char chan_name[50];
  for(chan = 0; chan < di->out; chan++) {
    sprintf(chan_name, "output_%ld", chan);
    printf("registering %s\n", chan_name);
    oport[chan] = jack_port_register (client, chan_name,
                                      JACK_DEFAULT_AUDIO_TYPE,
                                      JackPortIsOutput, chan);

//    if (oport[chan] == NULL)
//        fprintf(stderr, "no more JACK output ports available\n");
  }

  for(chan = 0; chan < di->in; chan++) {
    sprintf(chan_name, "input_%ld", chan);
    iport[chan] = jack_port_register (client, chan_name,
                                      JACK_DEFAULT_AUDIO_TYPE,
                                      JackPortIsInput, chan);

    /*
        if (iport[chan] == NULL)
    		{
            fprintf(stderr, "no more JACK input ports available\n");
          return -1;
        }
    */
  }
  di->sr = jack_get_sample_rate(client);
  return 1;
}

void jack_run(VM* vm, DriverInfo* di)
{
  const char** ports;
  m_uint chan;
  if (jack_activate (client)) {
    fprintf (stderr, "cannot activate client\n");
    return;
  }
  ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical | JackPortIsInput);
  if (!ports) {
    fprintf(stderr, "no physical playback ports\n");
    return;
  }
  for(chan = 0; chan < di->out; chan++) {
    printf("%s\n", ports[chan]);
    if(jack_connect(client, jack_port_name (oport[chan]), ports[chan]))
      fprintf (stderr, "cannot connect output ports\n");
  }
  free(ports);

  ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical | JackPortIsOutput);
  if (ports == NULL) {
    fprintf(stderr, "no physical record ports\n");
    di->in = 0;
  }
  for(chan = 0; chan < di->in; chan++) {
    if(jack_connect(client, ports[chan], jack_port_name (iport[chan]))) {
      fprintf (stderr, "cannot connect output ports\n");
    }
  }

  free(ports);

  while(vm->is_running) usleep(10);

}

void jack_del()
{
  jack_client_close(client);
  free(iport);
  free(oport);
}

Driver* jack_driver(VM* vm)
{
  Driver* d = malloc(sizeof(Driver));
  d->ini = jack_ini;
  d->run = jack_run;
  d->del = jack_del;
  vm->wakeup = jack_wakeup;
  return d;
}
