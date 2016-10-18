#include <stdlib.h>
#include <jack/jack.h>

#include <unistd.h> // for sleep

#include "vm.h"
#include "bbq.h"
#include "driver.h"
jack_port_t** iport;
jack_port_t** oport;
jack_client_t* client;

extern m_bool ssp_is_running;
static void gwion_shutdown (void *arg)
{
	ssp_is_running = 0;
}

static int gwion_cb(jack_nframes_t nframes, void *arg)
{
	int i, chan;
	VM* vm  = (VM*)arg;
	jack_default_audio_sample_t  *out[vm->bbq->sp->nchan];
	for(chan = 0; chan < vm->bbq->sp->nchan; chan++)
		out[chan] = jack_port_get_buffer (oport[chan], nframes);
	for(i = 0; i < nframes; i++)
	{
		vm_run(vm);
		for(chan = 0; chan < vm->bbq->sp->nchan; chan++)
			out[chan][i] = vm->bbq->sp->out[chan];
	}
	return 0;
}

m_bool jack_ini(VM* vm, DriverInfo* di)
//m_bool jack_ini(VM* vm, m_uint* i, m_uint* o, m_uint* samplerate)
{
	const char** ports;
	m_str  client_name = "Gwion";
	const m_str  server_name = NULL;
	m_uint chan;
	jack_options_t options = JackNullOption;
	jack_status_t status;

	iport = malloc(sizeof(jack_port_t *) * di->in);
	oport = malloc(sizeof(jack_port_t *) * di->out);
//	client = malloc(sizeof(jack_client_t *));
	client = jack_client_open (client_name, options, &status, server_name);
	if(!client)
	{
		fprintf (stderr, "jack_client_open() failed, "
			"status = 0x%2.0x\n", status);
		if (status & JackServerFailed)
			fprintf (stderr, "Unable to connect to JACK server\n");
		return -1;
	}
	if (status & JackNameNotUnique)
	{
		client_name = jack_get_client_name(client);
		fprintf (stderr, "unique name `%s' assigned\n", client_name);
	}
	jack_set_process_callback (client, gwion_cb, vm);
  jack_on_shutdown (client, gwion_shutdown, 0);

	char chan_name[50];
	for(chan = 0; chan < di->out; chan++)
	{
    sprintf(chan_name, "output_%d", chan);
    printf("registering %s\n", chan_name);
    oport[chan] = jack_port_register (client, chan_name,
                      JACK_DEFAULT_AUDIO_TYPE,
                      JackPortIsOutput, chan);

    if (oport[chan] == NULL)
		{
        fprintf(stderr, "no more JACK output ports available\n");
//        exit (1);
    }
	}

	for(chan = 0; chan < di->in; chan++)
	{
    sprintf(chan_name, "input_%d", chan);
    printf("registering %s\n", chan_name);
    oport[chan] = jack_port_register (client, chan_name,
                      JACK_DEFAULT_AUDIO_TYPE,
                      JackPortIsInput, chan);

    if (oport[chan] == NULL)
		{
        fprintf(stderr, "no more JACK input ports available\n");
      return -1;
    }
	}

	ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical|JackPortIsInput);
	if (ports == NULL) {
		fprintf(stderr, "no physical playback ports\n");
		exit (1);
	}
	for(chan = 0; chan < di->out; chan++)
	{
    if(jack_connect(client, jack_port_name (oport[chan]), ports[chan]))
		{
 //       fprintf (stderr, "cannot connect output ports\n");
    }
	}
	free(ports);

	ports = jack_get_ports(client, NULL, NULL, JackPortIsPhysical|JackPortIsOutput);
	if (ports == NULL) {
		fprintf(stderr, "no physical record ports\n");
//		exit (1);
	}
	for(chan = 0; chan < di->in; chan++)
	{
    if(jack_connect(client, jack_port_name (iport[chan]), ports[chan]))
		{
 //       fprintf (stderr, "cannot connect output ports\n");
    }
	}
	free(ports);

	if (jack_activate (client))
	{
		fprintf (stderr, "cannot activate client");
		return -1;
	}
	di->sr = jack_get_sample_rate(client);
	return 1;
}

void jack_run()
{
	while(ssp_is_running)
		sleep(1);
}

void jack_del()
{
	jack_client_close(client);
	free(iport);
	free(oport);
}

Driver* jack_driver()
{
	Driver* d = malloc(sizeof(Driver));
	d->ini = jack_ini;
	d->run = jack_run;
	d->del = jack_del;
}
