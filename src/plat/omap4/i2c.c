/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

#include <platsupport/i2c.h>


/* For OMAP3 I2C_IV has changed to I2C_WE (wakeup enable) */
enum {
        OMAP_I2C_REV_REG = 0,
        OMAP_I2C_IE_REG,
        OMAP_I2C_STAT_REG,
        OMAP_I2C_IV_REG,
        OMAP_I2C_WE_REG,
        OMAP_I2C_SYSS_REG,
        OMAP_I2C_BUF_REG,
        OMAP_I2C_CNT_REG,
        OMAP_I2C_DATA_REG,
        OMAP_I2C_SYSC_REG,
        OMAP_I2C_CON_REG,
        OMAP_I2C_OA_REG,
        OMAP_I2C_SA_REG,
        OMAP_I2C_PSC_REG,
        OMAP_I2C_SCLL_REG,
        OMAP_I2C_SCLH_REG,
        OMAP_I2C_SYSTEST_REG,
        OMAP_I2C_BUFSTAT_REG,
        /* only on OMAP4430 */
        OMAP_I2C_IP_V2_REVNB_LO,
        OMAP_I2C_IP_V2_REVNB_HI,
        OMAP_I2C_IP_V2_IRQSTATUS_RAW,
        OMAP_I2C_IP_V2_IRQENABLE_SET,
        OMAP_I2C_IP_V2_IRQENABLE_CLR,
};

/* I2C controller revisions */
#define OMAP_I2C_OMAP1_REV_2            0x20

/* I2C controller revisions present on specific hardware */
#define OMAP_I2C_REV_ON_2430            0x00000036
#define OMAP_I2C_REV_ON_3430_3530       0x0000003C
#define OMAP_I2C_REV_ON_3630            0x00000040
#define OMAP_I2C_REV_ON_4430_PLUS       0x50400002

/* I2C Interrupt Enable Register (OMAP_I2C_IE): */
#define OMAP_I2C_IE_XDR         (1 << 14)       /* TX Buffer drain int enable */
#define OMAP_I2C_IE_RDR         (1 << 13)       /* RX Buffer drain int enable */
#define OMAP_I2C_IE_XRDY        (1 << 4)        /* TX data ready int enable */
#define OMAP_I2C_IE_RRDY        (1 << 3)        /* RX data ready int enable */
#define OMAP_I2C_IE_ARDY        (1 << 2)        /* Access ready int enable */
#define OMAP_I2C_IE_NACK        (1 << 1)        /* No ack interrupt enable */
#define OMAP_I2C_IE_AL          (1 << 0)        /* Arbitration lost int ena */

/* I2C Status Register (OMAP_I2C_STAT): */
#define OMAP_I2C_STAT_XDR       (1 << 14)       /* TX Buffer draining */
#define OMAP_I2C_STAT_RDR       (1 << 13)       /* RX Buffer draining */
#define OMAP_I2C_STAT_BB        (1 << 12)       /* Bus busy */
#define OMAP_I2C_STAT_ROVR      (1 << 11)       /* Receive overrun */
#define OMAP_I2C_STAT_XUDF      (1 << 10)       /* Transmit underflow */
#define OMAP_I2C_STAT_AAS       (1 << 9)        /* Address as slave */
#define OMAP_I2C_STAT_BF        (1 << 8)        /* Bus Free */
#define OMAP_I2C_STAT_XRDY      (1 << 4)        /* Transmit data ready */
#define OMAP_I2C_STAT_RRDY      (1 << 3)        /* Receive data ready */
#define OMAP_I2C_STAT_ARDY      (1 << 2)        /* Register access ready */
#define OMAP_I2C_STAT_NACK      (1 << 1)        /* No ack interrupt enable */
#define OMAP_I2C_STAT_AL        (1 << 0)        /* Arbitration lost int ena */

/* I2C WE wakeup enable register */
#define OMAP_I2C_WE_XDR_WE      (1 << 14)       /* TX drain wakup */
#define OMAP_I2C_WE_RDR_WE      (1 << 13)       /* RX drain wakeup */
#define OMAP_I2C_WE_AAS_WE      (1 << 9)        /* Address as slave wakeup*/
#define OMAP_I2C_WE_BF_WE       (1 << 8)        /* Bus free wakeup */
#define OMAP_I2C_WE_STC_WE      (1 << 6)        /* Start condition wakeup */
#define OMAP_I2C_WE_GC_WE       (1 << 5)        /* General call wakeup */
#define OMAP_I2C_WE_DRDY_WE     (1 << 3)        /* TX/RX data ready wakeup */
#define OMAP_I2C_WE_ARDY_WE     (1 << 2)        /* Reg access ready wakeup */
#define OMAP_I2C_WE_NACK_WE     (1 << 1)        /* No acknowledgment wakeup */
#define OMAP_I2C_WE_AL_WE       (1 << 0)        /* Arbitration lost wakeup */

#define OMAP_I2C_WE_ALL         (OMAP_I2C_WE_XDR_WE | OMAP_I2C_WE_RDR_WE | \
        OMAP_I2C_WE_AAS_WE | OMAP_I2C_WE_BF_WE | \
        OMAP_I2C_WE_STC_WE | OMAP_I2C_WE_GC_WE | \
        OMAP_I2C_WE_DRDY_WE | OMAP_I2C_WE_ARDY_WE | \
        OMAP_I2C_WE_NACK_WE | OMAP_I2C_WE_AL_WE)

/* I2C Buffer Configuration Register (OMAP_I2C_BUF): */
#define OMAP_I2C_BUF_RDMA_EN    (1 << 15)       /* RX DMA channel enable */
#define OMAP_I2C_BUF_RXFIF_CLR  (1 << 14)       /* RX FIFO Clear */
#define OMAP_I2C_BUF_XDMA_EN    (1 << 7)        /* TX DMA channel enable */
#define OMAP_I2C_BUF_TXFIF_CLR  (1 << 6)        /* TX FIFO Clear */

/* I2C Configuration Register (OMAP_I2C_CON): */
#define OMAP_I2C_CON_EN         (1 << 15)       /* I2C module enable */
#define OMAP_I2C_CON_BE         (1 << 14)       /* Big endian mode */
#define OMAP_I2C_CON_OPMODE_HS  (1 << 12)       /* High Speed support */
#define OMAP_I2C_CON_STB        (1 << 11)       /* Start byte mode (master) */
#define OMAP_I2C_CON_MST        (1 << 10)       /* Master/slave mode */
#define OMAP_I2C_CON_TRX        (1 << 9)        /* TX/RX mode (master only) */
#define OMAP_I2C_CON_XA         (1 << 8)        /* Expand address */
#define OMAP_I2C_CON_RM         (1 << 2)        /* Repeat mode (master only) */
#define OMAP_I2C_CON_STP        (1 << 1)        /* Stop cond (master only) */
#define OMAP_I2C_CON_STT        (1 << 0)        /* Start condition (master) */

/* I2C SCL time value when Master */
#define OMAP_I2C_SCLL_HSSCLL    8
#define OMAP_I2C_SCLH_HSSCLH    8

/* I2C System Test Register (OMAP_I2C_SYSTEST): */
#define OMAP_I2C_SYSTEST_ST_EN          (1 << 15)       /* System test enable */
#define OMAP_I2C_SYSTEST_FREE           (1 << 14)       /* Free running mode */
#define OMAP_I2C_SYSTEST_TMODE_MASK     (3 << 12)       /* Test mode select */
#define OMAP_I2C_SYSTEST_TMODE_SHIFT    (12)            /* Test mode select */
/* Functional mode */
#define OMAP_I2C_SYSTEST_SCL_I_FUNC     (1 << 8)        /* SCL line input value */
#define OMAP_I2C_SYSTEST_SCL_O_FUNC     (1 << 7)        /* SCL line output value */
#define OMAP_I2C_SYSTEST_SDA_I_FUNC     (1 << 6)        /* SDA line input value */
#define OMAP_I2C_SYSTEST_SDA_O_FUNC     (1 << 5)        /* SDA line output value */
/* SDA/SCL IO mode */
#define OMAP_I2C_SYSTEST_SCL_I          (1 << 3)        /* SCL line sense in */
#define OMAP_I2C_SYSTEST_SCL_O          (1 << 2)        /* SCL line drive out */
#define OMAP_I2C_SYSTEST_SDA_I          (1 << 1)        /* SDA line sense in */
#define OMAP_I2C_SYSTEST_SDA_O          (1 << 0)        /* SDA line drive out */

/* OCP_SYSSTATUS bit definitions */
#define SYSS_RESETDONE_MASK             (1 << 0)

/* OCP_SYSCONFIG bit definitions */
#define SYSC_CLOCKACTIVITY_MASK         (0x3 << 8)
#define SYSC_SIDLEMODE_MASK             (0x3 << 3)
#define SYSC_ENAWAKEUP_MASK             (1 << 2)
#define SYSC_SOFTRESET_MASK             (1 << 1)
#define SYSC_AUTOIDLE_MASK              (1 << 0)

#define SYSC_IDLEMODE_SMART             0x2
#define SYSC_CLOCKACTIVITY_FCLK         0x2


static const u8 reg_map_ip_v2[] =
{
        [OMAP_I2C_REV_REG] = 0x04,
        [OMAP_I2C_IE_REG] = 0x2c,
        [OMAP_I2C_STAT_REG] = 0x28,
        [OMAP_I2C_IV_REG] = 0x34,
        [OMAP_I2C_WE_REG] = 0x34,
        [OMAP_I2C_SYSS_REG] = 0x90,
        [OMAP_I2C_BUF_REG] = 0x94,
        [OMAP_I2C_CNT_REG] = 0x98,
        [OMAP_I2C_DATA_REG] = 0x9c,
        [OMAP_I2C_SYSC_REG] = 0x10,
        [OMAP_I2C_CON_REG] = 0xa4,
        [OMAP_I2C_OA_REG] = 0xa8,
        [OMAP_I2C_SA_REG] = 0xac,
        [OMAP_I2C_PSC_REG] = 0xb0,
        [OMAP_I2C_SCLL_REG] = 0xb4,
        [OMAP_I2C_SCLH_REG] = 0xb8,
        [OMAP_I2C_SYSTEST_REG] = 0xbC,
        [OMAP_I2C_BUFSTAT_REG] = 0xc0,
        [OMAP_I2C_IP_V2_REVNB_LO] = 0x00,
        [OMAP_I2C_IP_V2_REVNB_HI] = 0x04,
        [OMAP_I2C_IP_V2_IRQSTATUS_RAW] = 0x24,
        [OMAP_I2C_IP_V2_IRQENABLE_SET] = 0x2c,
        [OMAP_I2C_IP_V2_IRQENABLE_CLR] = 0x30,
};

struct omap_i2c_dev {
        //struct device           *dev;
        void                    *base;          /* virtual */
        int                     irq;
        int                     reg_shift;      /* bit shift for I2C register addresses */
        //struct completion       cmd_complete;
        //struct resource         *ioarea;
        u32                     latency;        /* maximum mpu wkup latency */
        //void                    (*set_mpu_wkup_lat)(struct device *dev, long latency);
        u32                     speed;          /* Speed of bus in kHz */
        u32                     flags;
        u16                     scheme;
        u16                     cmd_err;
        u8                      *buf;
        u8                      *regs;
        size_t                  buf_len;
        //struct i2c_adapter      adapter;
        u8                      threshold;
        u8                      fifo_size;      /* use as flag and value
                                                 * fifo_size==0 implies no fifo
                                                 * if set, should be trsh+1
                                                 */
        u32                     rev;
        unsigned                b_hw:1;         /* bad h/w fixes */
        unsigned                bb_valid:1;     /* true when BB-bit reflects
                                                 * the I2C bus state
                                                 */
        unsigned                receiver:1;     /* true when we're in receiver mode */
        u16                     iestate;        /* Saved interrupt register */
        u16                     pscstate;
        u16                     scllstate;
        u16                     sclhstate;
        u16                     syscstate;
        u16                     westate;
        u16                     errata;
};

#define HWREG(x)                (*((volatile unsigned int *)(x)))

static inline void omap_i2c_write_reg(struct omap_i2c_dev *omap, int reg, u16 val)
{
        HWREG(val) = omap->base + (omap->regs[reg] << omap->reg_shift);
}

static inline u16 omap_i2c_read_reg(struct omap_i2c_dev *omap, int reg)
{
        return HWREG(omap->base + (omap->regs[reg] << omap->reg_shift));
}

static int omap_i2c_init(struct omap_i2c_dev *omap)
{
        u16 psc = 0, scll = 0, sclh = 0;
        u16 fsscll = 0, fssclh = 0, hsscll = 0, hssclh = 0;
        unsigned long fclk_rate = 12000000;
        unsigned long internal_clk = 0;
        struct clk *fclk;

        if (omap->rev >= OMAP_I2C_REV_ON_3430_3530) {
                /*
                 * Enabling all wakup sources to stop I2C freezing on
                 * WFI instruction.
                 * REVISIT: Some wkup sources might not be needed.
                 */
                omap->westate = OMAP_I2C_WE_ALL;
        }

        if (omap->flags & OMAP_I2C_FLAG_ALWAYS_ARMXOR_CLK) {
                /*
                 * The I2C functional clock is the armxor_ck, so there's
                 * no need to get "armxor_ck" separately.  Now, if OMAP2420
                 * always returns 12MHz for the functional clock, we can
                 * do this bit unconditionally.
                 */
                fclk = clk_get(omap->dev, "fck");
                fclk_rate = clk_get_rate(fclk);
                clk_put(fclk);

                /* TRM for 5912 says the I2C clock must be prescaled to be
                 * between 7 - 12 MHz. The XOR input clock is typically
                 * 12, 13 or 19.2 MHz. So we should have code that produces:
                 *
                 * XOR MHz      Divider         Prescaler
                 * 12           1               0
                 * 13           2               1
                 * 19.2         2               1
                 */
                if (fclk_rate > 12000000)
                        psc = fclk_rate / 12000000;
        }

        if (!(omap->flags & OMAP_I2C_FLAG_SIMPLE_CLOCK)) {

                /*
                 * HSI2C controller internal clk rate should be 19.2 Mhz for
                 * HS and for all modes on 2430. On 34xx we can use lower rate
                 * to get longer filter period for better noise suppression.
                 * The filter is iclk (fclk for HS) period.
                 */
                if (omap->speed > 400 ||
                               omap->flags & OMAP_I2C_FLAG_FORCE_19200_INT_CLK)
                        internal_clk = 19200;
                else if (omap->speed > 100)
                        internal_clk = 9600;
                else
                        internal_clk = 4000;
                fclk = clk_get(omap->dev, "fck");
                fclk_rate = clk_get_rate(fclk) / 1000;
                clk_put(fclk);

                /* Compute prescaler divisor */
                psc = fclk_rate / internal_clk;
                psc = psc - 1;

                /* If configured for High Speed */
                if (omap->speed > 400) {
                        unsigned long scl;

                        /* For first phase of HS mode */
                        scl = internal_clk / 400;
                        fsscll = scl - (scl / 3) - 7;
                        fssclh = (scl / 3) - 5;

                        /* For second phase of HS mode */
                        scl = fclk_rate / omap->speed;
                        hsscll = scl - (scl / 3) - 7;
                        hssclh = (scl / 3) - 5;
                } else if (omap->speed > 100) {
                        unsigned long scl;

                        /* Fast mode */
                        scl = internal_clk / omap->speed;
                        fsscll = scl - (scl / 3) - 7;
                        fssclh = (scl / 3) - 5;
                } else {
                        /* Standard mode */
                        fsscll = internal_clk / (omap->speed * 2) - 7;
                        fssclh = internal_clk / (omap->speed * 2) - 5;
                }
                scll = (hsscll << OMAP_I2C_SCLL_HSSCLL) | fsscll;
                sclh = (hssclh << OMAP_I2C_SCLH_HSSCLH) | fssclh;
        } else {
                /* Program desired operating rate */
                fclk_rate /= (psc + 1) * 1000;
                if (psc > 2)
                        psc = 2;
                scll = fclk_rate / (omap->speed * 2) - 7 + psc;
                sclh = fclk_rate / (omap->speed * 2) - 7 + psc;
        }

        omap->iestate = (OMAP_I2C_IE_XRDY | OMAP_I2C_IE_RRDY |
                        OMAP_I2C_IE_ARDY | OMAP_I2C_IE_NACK |
                        OMAP_I2C_IE_AL)  | ((omap->fifo_size) ?
                                (OMAP_I2C_IE_RDR | OMAP_I2C_IE_XDR) : 0);

        omap->pscstate = psc;
        omap->scllstate = scll;
        omap->sclhstate = sclh;

        if (omap->rev <= OMAP_I2C_REV_ON_3430_3530) {
                /* Not implemented */
                omap->bb_valid = 1;
        }

        __omap_i2c_init(omap);

        return 0;
}
