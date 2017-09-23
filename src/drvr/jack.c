#include <stdlib.h>
#include <jack/jack.h>

#include <unistd.h> // for sleep

#include "vm.h"
#include "driver.h"
jack_port_t** iport;
jack_port_t** oport;
jack_client_t* client;

void jack_wakeup() {
//	jack_client_close(client);
  jack_deactivate(client);
}

static void gwion_shutdown(void *arg) {
  VM *vm = (VM *)arg;
  vm->is_running = 0;
}

static void inner_cb(VM* vm, jack_default_audio_sample_t** in,
    jack_default_audio_sample_t** out, jack_nframes_t nframes) {
  jack_nframes_t frame;
  sp_data* sp = vm->sp;
  m_uint chan;
  for(frame = 0; frame < nframes; frame++) {
    for(chan = 0; chan < vm->n_in; chan++)
      vm->in[chan] = in[chan][frame];
    di->run(vm);
    for(chan = 0; chan < sp->nchan; chan++)
      out[chan][frame] = sp->out[chan];
    sp->pos++;
  }
}

static int gwion_cb(jack_nframes_t nframes, void *arg) {
  int chan;
  VM* vm  = (VM*)arg;
  sp_data* sp = vm->sp;
  jack_default_audio_sample_t  * in[vm->n_in];
  jack_default_audio_sample_t  * out[sp->nchan];
  for(chan = 0; chan < vm->n_in; chan++)
    in[chan] = jack_port_get_buffer(iport[chan], nframes);
  for(chan = 0; chan < sp->nchan; chan++)
    out[chan] = jack_port_get_buffer(oport[chan], nframes);
  inner_cb(vm, in, out, nframes);
  GWION_CTL
  return 0;
}

static m_bool init_client() {
  jack_status_t status;
  client = jack_client_open("Gwion", JackNullOption, &status, NULL);
  if(!client) {
    fprintf(stderr, "jack_client_open() failed, status = 0x%2.0x\n", status);
    if(status & JackServerFailed)
      fprintf(stderr, "Unable to connect to JACK server\n");
    return -1;
  }
  jack_set_process_callback(client, gwion_cb, vm);
  jack_on_shutdown(client, gwion_shutdown, vm);
  return 1;
}

static m_bool set_chan(m_uint nchan, m_bool input) {
  char chan_name[50];
  m_uint chan;
  jack_port_t** port = input ? iport : oport;
  for(chan = 0; chan < nchan; chan++) {
    sprintf(chan_name, input ? "input_%ld" : "output_%ld", chan);
    if(!(port[chan] = jack_port_register(client, chan_name,
        JACK_DEFAULT_AUDIO_TYPE, input ? 
        JackPortIsInput : JackPortIsOutput , chan))) {
      fprintf(stderr, "no more JACK %s ports available\n", input ? 
          "input" : "output");
      return -1;
    }
  }
  return 1;
}

static m_bool jack_ini(VM* vm, DriverInfo* di) {
  iport = malloc(sizeof(jack_port_t *) * di->in);
  oport = malloc(sizeof(jack_port_t *) * di->out);
  CHECK_BB(init_client())
  CHECK_BB(set_chan(di->out, 0))
  CHECK_BB(set_chan(di->in,  1))
  di->sr = jack_get_sample_rate(client);
  return 1;
}

static m_bool connect_ports(const char** ports, m_uint nchan, m_bool input) {
  m_uint chan;
  jack_port_t** jport = input ? iport : oport;
  for(chan = 0; chan < nchan; chan++) {
    const char* l = input ? ports[chan] : jack_port_name(jport[chan]);
    const char* r = input ? jack_port_name(jport[chan]) : ports[chan];
    if(jack_connect(client, l, r)) {
      fprintf(stderr, "cannot connect %s ports\n", input ? "input" : "output");
      return -1;
    }
  }
  return 1;
}

static m_bool init_ports(m_uint nchan, m_bool input) {
  m_bool ret;
  const char** ports = jack_get_ports(client, NULL, NULL,
      JackPortIsPhysical | (input ? JackPortIsOutput : JackPortIsInput));
  if(!ports)
    return - 1;
  ret = connect_ports(ports, nchan, input);
  free(ports);
  return ret;
}

static void jack_run(VM* vm, DriverInfo* di) {
  if(jack_activate(client)) {
    fprintf(stderr, "cannot activate client\n");
    return;
  }
  if(init_ports(di->out, 0) < 0 || init_ports(di->in,  1) < 0)
    return;
  while(vm->is_running)
    usleep(10);
}

static void jack_del() {
  jack_client_close(client);
  free(iport);
  free(oport);
}

void jack_driver(Driver* d, VM* vm) {
  d->ini = jack_ini;
  d->run = jack_run;
  d->del = jack_del;
  vm->wakeup = jack_wakeup;
}
