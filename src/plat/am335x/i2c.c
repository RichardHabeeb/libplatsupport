/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

#include <stdlib.h>
#include <platsupport/i2c.h>

int am335x_i2c_init(enum i2c_id id, i2c_bus_t* i2c)
{
  i2c->start_read  = am335x_i2c_start_read;
  i2c->start_write = am335x_i2c_start_write;
  i2c->read        = am335x_i2c_read;
  i2c->write       = am335x_i2c_write;
  i2c->set_speed   = am335x_i2c_set_speed;
  i2c->set_address = am335x_i2c_set_address;
  i2c->probe_aas   = am335x_i2c_probe_aas;
  i2c->master_stop = am335x_i2c_master_stop;
  i2c->handle_irq  = am335x_i2c_handle_irq;
  //i2c->priv        = (void*)dev;
}


int am335x_i2c_start_read(i2c_bus_t* bus, int slave, void* buf, size_t size, i2c_callback_fn cb, void* token)
{
}

int am335x_i2c_start_write(i2c_bus_t* bus, int slave, const void* buf, size_t size, i2c_callback_fn cb, void* token)
{
}

int am335x_i2c_read(i2c_bus_t* bus, void* buf, size_t size, i2c_callback_fn cb, void* token)
{
}

int am335x_i2c_write(i2c_bus_t* bus, const void* buf, size_t size, i2c_callback_fn cb, void* token)
{
}

long am335x_i2c_set_speed(i2c_bus_t* bus, long bps)
{
}

int am335x_i2c_master_stop(i2c_bus_t* bus)
{
}

int am335x_i2c_set_address(i2c_bus_t* bus, int addr, i2c_aas_callback_fn aas_cb, void* aas_token)
{
}

enum i2c_mode am335x_i2c_probe_aas(i2c_bus_t* bus)
{
}

void am335x_i2c_handle_irq(i2c_bus_t* bus)
{
}
