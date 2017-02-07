
/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

#ifndef _PLATSUPPORT_PLAT_I2C_H_
#define _PLATSUPPORT_PLAT_I2C_H_

enum i2c_id {
    NI2C
};

int omap4_i2c_init(void* base, i2c_bus_t* i2c);

#endif /* _PLATSUPPORT_PLAT_I2C_H_ */
