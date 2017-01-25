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


// struct __attribute__((__packed__)) am335x_i2c_regs
// {
//   uint32_t I2C_REVNB_LO;        /* This read-only register contains the hard-coded revision number of the module */
//   uint32_t I2C_REVNB_HI;        /* This read-only register contains the hard-coded revision number of the module */
//   uint16_t pad0;
//   uint32_t I2C_SYSC;            /* This register allows controlling various parameters of the peripheral interface */
//   uint16_t pad1;
//   uint32_t pad2;
//   uint32_t pad3;
//   uint32_t I2C_IRQSTATUS_RAW;   /* This register provides core status information for interrupt handling, showing all active events */
//   uint32_t I2C_IRQSTATUS;       /* This register provides core status information for interrupt handling, showing all active and enabled events and masking the others */
//   uint32_t I2C_IRQENABLE_SET;   /* All 1-bit fields enable a specific interrupt event to trigger an interrupt request */
//   uint32_t I2C_IRQENABLE_CLR;   /* All 1-bit fields clear a specific interrupt event. */
//   uint32_t I2C_WE;              /* Every 1-bit field in the I2C_WE register enables a specific (synchronous) IRQ request source to generate an asynchronous wakeup (on the appropriate swakeup line). */
//   uint32_t I2C_DMARXENABLE_SET; /* The 1-bit field enables a receive DMA request. */
//   uint32_t I2C_DMATXENABLE_SET; /* The 1-bit field enables a transmit DMA request. */
//   uint32_t I2C_DMARXENABLE_CLR; /* The 1-bit field disables a receive DMA request. */
//   uint32_t I2C_DMATXENABLE_CLR; /* The 1-bit field disables a transmit DMA request. */
//   uint32_t I2C_DMARXWAKE_EN;    /* All 1-bit fields enable a specific (synchronous) DMA request source to generate an asynchronous wakeup (on the appropriate swakeup line) */
//   uint32_t I2C_DMATXWAKE_EN;    /* All 1-bit fields enable a specific (synchronous) DMA request source to generate an asynchronous wakeup (on the appropriate swakeup line) */
//   uint32_t pad4[11];
//   uint32_t I2C_SYSS;            /* This read-only bit indicates the state of the reset in case of hardware reset, global software reset (I2C_SYSC.SRST) or partial software reset (I2C_CON.I2C_EN). */
//   uint32_t I2C_BUF;             /* This read/write register enables DMA transfers and allows the configuration of FIFO thresholds for the FIFO management (see the FIFO Management subsection). */
//   uint32_t I2C_CNT;             /* A read returns the number of bytes that are yet to be received or sent. CAUTION: During an active transfer phase (between STT having been set to 1 and reception of ARDY), no modification must be done in this register. */
// }

/****************************************************************************/
/*
 ** Register definition for I2C peripherals
 */

#define I2C_REVNB_LO          (0x0)
#define I2C_REVNB_HI          (0x4)
#define I2C_SYSC              (0x10)
#define I2C_EOI               (0x20)
#define I2C_IRQSTATUS_RAW     (0x24)
#define I2C_IRQSTATUS         (0x28)
#define I2C_IRQENABLE_SET     (0x2C)
#define I2C_IRQENABLE_CLR     (0x30)
#define I2C_WE                (0x34)
#define I2C_DMARXENABLE_SET   (0x38)
#define I2C_DMATXENABLE_SET   (0x3C)
#define I2C_DMARXENABLE_CLR   (0x40)
#define I2C_DMATXENABLE_CLR   (0x44)
#define I2C_DMARXWAKE_EN      (0x48)
#define I2C_DMATXWAKE_EN      (0x4C)
#define I2C_SYSS              (0x90)
#define I2C_BUF               (0x94)
#define I2C_CNT               (0x98)
#define I2C_DATA              (0x9C)
#define I2C_CON               (0xA4)
#define I2C_OA                (0xA8)
#define I2C_SA                (0xAC)
#define I2C_PSC               (0xB0)
#define I2C_SCLL              (0xB4)
#define I2C_SCLH              (0xB8)
#define I2C_SYSTEST           (0xBC)
#define I2C_BUFSTAT           (0xC0)
#define I2C_OAn(n)            (0xC4 + ((n) * 4))
#define I2C_ACTOA             (0xD0)
#define I2C_SBLOCK            (0xD4)

/**************************************************************************\
 * Field Definition Macros
\**************************************************************************/

// SYSC
#define I2C_SYSC_AUTOIDLE     (0x00000001u)
#define I2C_SYSC_CLKACTIVITY  (0x00000300u)
#define I2C_SYSC_ENAWAKEUP    (0x00000004u)
#define I2C_SYSC_IDLEMODE     (0x00000018u)
#define I2C_SYSC_SRST         (0x00000002u)

// IRQSTATUS_RAW
#define I2C_IRQSTATUS_RAW_AAS    (0x00000200u)
#define I2C_IRQSTATUS_RAW_AERR   (0x00000080u)
#define I2C_IRQSTATUS_RAW_AL     (0x00000001u)
#define I2C_IRQSTATUS_RAW_ARDY   (0x00000004u)
#define I2C_IRQSTATUS_RAW_BB     (0x00001000u)
#define I2C_IRQSTATUS_RAW_BF     (0x00000100u)
#define I2C_IRQSTATUS_RAW_GC     (0x00000020u)
#define I2C_IRQSTATUS_RAW_NACK   (0x00000002u)
#define I2C_IRQSTATUS_RAW_RDR    (0x00002000u)
#define I2C_IRQSTATUS_RAW_ROVR   (0x00000800u)
#define I2C_IRQSTATUS_RAW_RRDY   (0x00000008u)
#define I2C_IRQSTATUS_RAW_STC    (0x00000040u)
#define I2C_IRQSTATUS_RAW_XDR    (0x00004000u)
#define I2C_IRQSTATUS_RAW_XRDY   (0x00000010u)
#define I2C_IRQSTATUS_RAW_XUDF   (0x00000400u)

// IRQSTATUS
#define I2C_IRQSTATUS_AAS  (0x00000200u)
#define I2C_IRQSTATUS_AERR (0x00000080u)
#define I2C_IRQSTATUS_AL   (0x00000001u)
#define I2C_IRQSTATUS_ARDY (0x00000004u)
#define I2C_IRQSTATUS_BB   (0x00001000u)
#define I2C_IRQSTATUS_BF   (0x00000100u)
#define I2C_IRQSTATUS_GC   (0x00000020u)
#define I2C_IRQSTATUS_NACK (0x00000002u)
#define I2C_IRQSTATUS_RDR  (0x00002000u)
#define I2C_IRQSTATUS_ROVR (0x00000800u)
#define I2C_IRQSTATUS_RRDY (0x00000008u)
#define I2C_IRQSTATUS_STC  (0x00000040u)
#define I2C_IRQSTATUS_XDR  (0x00004000u)
#define I2C_IRQSTATUS_XRDY (0x00000010u)
#define I2C_IRQSTATUS_XUDF (0x00000400u)

// IRQENABLE_SET
#define I2C_IRQENABLE_SET_AERR_IE   (0x00000080u)
#define I2C_IRQENABLE_SET_AL_IE     (0x00000001u)
#define I2C_IRQENABLE_SET_ARDY_IE   (0x00000004u)
#define I2C_IRQENABLE_SET_ASS_IE    (0x00000200u)
#define I2C_IRQENABLE_SET_BF_IE     (0x00000100u)
#define I2C_IRQENABLE_SET_GC_IE     (0x00000020u)
#define I2C_IRQENABLE_SET_NACK_IE   (0x00000002u)
#define I2C_IRQENABLE_SET_RDR_IE    (0x00002000u)
#define I2C_IRQENABLE_SET_ROVR      (0x00000800u)
#define I2C_IRQENABLE_SET_RRDY_IE   (0x00000008u)
#define I2C_IRQENABLE_SET_STC_IE    (0x00000040u)
#define I2C_IRQENABLE_SET_XDR_IE    (0x00004000u)
#define I2C_IRQENABLE_SET_XRDY_IE   (0x00000010u)
#define I2C_IRQENABLE_SET_XUDF      (0x00000400u)

/* IRQENABLE_CLR */
#define I2C_IRQENABLE_CLR_AERR_IE   (0x00000080u)
#define I2C_IRQENABLE_CLR_AL_IE     (0x00000001u)
#define I2C_IRQENABLE_CLR_ARDY_IE   (0x00000004u)
#define I2C_IRQENABLE_CLR_ASS_IE    (0x00000200u)
#define I2C_IRQENABLE_CLR_BF_IE     (0x00000100u)
#define I2C_IRQENABLE_CLR_GC_IE     (0x00000020u)
#define I2C_IRQENABLE_CLR_NACK_IE   (0x00000002u)
#define I2C_IRQENABLE_CLR_RDR_IE    (0x00002000u)
#define I2C_IRQENABLE_CLR_ROVR      (0x00000800u)
#define I2C_IRQENABLE_CLR_RRDY_IE   (0x00000008u)
#define I2C_IRQENABLE_CLR_STC_IE    (0x00000040u)
#define I2C_IRQENABLE_CLR_XDR_IE    (0x00004000u)
#define I2C_IRQENABLE_CLR_XRDY_IE   (0x00000010u)
#define I2C_IRQENABLE_CLR_XUDF      (0x00000400u)

/* SYSS */
#define I2C_SYSS_RDONE  (0x00000001u)

/* CON */
#define I2C_CON_I2C_EN  (0x00008000u)
#define I2C_CON_MST     (0x00000400u)
#define I2C_CON_OPMODE  (0x00003000u)
#define I2C_CON_STB     (0x00000800u)
#define I2C_CON_STP     (0x00000002u)
#define I2C_CON_STT     (0x00000001u)
#define I2C_CON_TRX     (0x00000200u)
#define I2C_CON_XOA0    (0x00000080u)
#define I2C_CON_XOA1    (0x00000040u)
#define I2C_CON_XOA2    (0x00000020u)
#define I2C_CON_XOA3    (0x00000010u)
#define I2C_CON_XSA     (0x00000100u)

/* PSC */
#define I2C_PSC_PSC     (0x000000FFu)

/* SCLL */
#define I2C_SCLL_HSSCLL   (0x0000FF00u)
#define I2C_SCLL_SCLL   (0x000000FFu)

/* SCLH */
#define I2C_SCLH_HSSCLH   (0x0000FF00u)
#define I2C_SCLH_SCLH   (0x000000FFu)

#define REG32(addr)  (*((volatile uint32_t*) (addr)))



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
