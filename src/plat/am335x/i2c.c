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


/****************************************************************************/
/*
 ** Values that can be passed to  I2CMasterControl API as cmd to configure mode
 ** of operation of I2C
 */

#define     I2C_CFG_MST_TX              I2C_CON_TRX | I2C_CON_MST
#define     I2C_CFG_MST_RX              I2C_CON_MST
#define     I2C_CFG_STOP                I2C_CON_STP
#define     I2C_CFG_N0RMAL_MODE         (0 << I2C_CON_STB_SHIFT)
#define     I2C_CFG_SRT_BYTE_MODE       I2C_CON_STB
#define     I2C_CFG_7BIT_SLAVE_ADDR     (0 << I2C_CON_XSA_SHIFT)
#define     I2C_CFG_10BIT_SLAVE_ADDR    I2C_CON_XSA
#define     I2C_CFG_10BIT_OWN_ADDR_0    I2C_CON_XOA0
#define     I2C_CFG_10BIT_OWN_ADDR_1    I2C_CON_XOA1
#define     I2C_CFG_10BIT_OWN_ADDR_2    I2C_CON_XOA2
#define     I2C_CFG_10BIT_OWN_ADDR_3    I2C_CON_XOA3
#define     I2C_CFG_7BIT_OWN_ADDR_0     (0 << I2C_CON_XOA0_SHIFT)
#define     I2C_CFG_7BIT_OWN_ADDR_1     (0 << I2C_CON_XOA1_SHIFT)
#define     I2C_CFG_7BIT_OWN_ADDR_2     (0 << I2C_CON_XOA2_SHIFT
#define     I2C_CFG_7BIT_OWN_ADDR_3     (0 << I2C_CON_XOA3_SHIFT)

/****************************************************************************/
/*
 ** Values that can be passed to  I2CMasterIntEnableEx API as intFlag to Enable
 ** interrupts.
 */

#define     I2C_INT_ARBITRATION_LOST     I2C_IRQSTATUS_AL
#define     I2C_INT_NO_ACK               I2C_IRQSTATUS_NACK
#define     I2C_INT_ADRR_READY_ACESS     I2C_IRQSTATUS_ARDY
#define     I2C_INT_RECV_READY           I2C_IRQSTATUS_RRDY
#define     I2C_INT_TRANSMIT_READY       I2C_IRQSTATUS_XRDY
#define     I2C_INT_GENERAL_CALL         I2C_IRQSTATUS_GC
#define     I2C_INT_START                I2C_IRQSTATUS_STC
#define     I2C_INT_ACCESS_ERROR         I2C_IRQSTATUS_AERR
#define     I2C_INT_STOP_CONDITION       I2C_IRQSTATUS_BF
#define     I2C_INT_ADRR_SLAVE           I2C_IRQSTATUS_AAS
#define     I2C_INT_TRANSMIT_UNDER_FLOW  I2C_IRQSTATUS_XUDF
#define     I2C_INT_RECV_OVER_RUN        I2C_IRQSTATUS_ROVR
#define     I2C_INT_RECV_DRAIN           I2C_IRQSTATUS_RDR
#define     I2C_INT_TRANSMIT_DRAIN       I2C_IRQSTATUS_XDR


#define REG32(addr)  (*((volatile uint32_t*) (addr)))



/**
 * \brief   This API will divide the system clock fed to I2C module between
 *          12 and 100Mhz.It will also configure the I2C bus clock frequency.
 *
 * \param   baseAdd     It is the Memory address of the I2C instance used.
 * \param   sysClk      It is the System clock fed to I2C module.
 * \param   internalClk It is the internal clock used by I2C module.Which is
 *                      obtained by scaling System clock fed to I2C module.
 * \param   outputClk   It is the required I2C bus speed or frequency.
 *
 * \return  None.
 **/
static void I2CMasterInitExpClk(uint32_t baseAdd, uint32_t sysClk, uint32_t internalClk, uint32_t outputClk)
{
   uint32_t prescalar;
   uint32_t divider;

   /* Calculate prescalar value */
   prescalar = (sysClk / internalClk) - 1;

   REG32(baseAdd + I2C_PSC) = prescalar;

   divider = internalClk/outputClk;

   divider = divider / 2;

   REG32(baseAdd + I2C_SCLL) = divider - 7;

   REG32(baseAdd + I2C_SCLH) = divider - 5;
}

/**
 * \brief   Enables the I2C module.This will bring the I2C module out of reset.
 *
 * \param   baseAdd   It is the Memory address of the I2C instance used.
 *
 * \return  None.
 *
 **/
static void I2CMasterEnable(uint32_t baseAdd)
{
   REG32(baseAdd + I2C_CON) |= I2C_CON_I2C_EN;
}

/**
 * \brief   Disables the I2C Module.This will put the I2C module in reset.
 *          Only Tx and Rx are cleared,status bits are set their default
 *          values and all configuration registers are not reset,they keep
 *          their initial values.
 *
 * \param   baseAdd   It is the Memory address of the I2C instance used.
 *
 * \return  None.
 *
 **/
static void I2CMasterDisable(uint32_t baseAdd)
{
   REG32(baseAdd + I2C_CON) &= ~(I2C_CON_I2C_EN);
}


/**
 * \brief   This API configure I2C in different modes of operation.
 *
 * \param   baseAdd   It is the Memory address of the I2C instance used.
 * \param   cmd       It is the value which configures I2C in different mode's
 *                    of operation.\n
 *
 *          cmd can take follwing macros.\n
 *
 *          I2C_CFG_MST_TX             - Configure's I2C as Master-Transmitter.\n
 *          I2C_CFG_MST_RX             - Configure's I2C as Master-Receiver.\n
 *          I2C_CFG_STOP               - Configure's I2C to generate stop
 *                                       condition when DCOUNT counts down to
 *                                       zero.\n
 *          I2C_CFG_N0RMAL_MODE        - Configure's I2C in normal mode.\n
 *          I2C_CFG_SRT_BYTE_MODE      - Configure's I2C in start byte mode.\n
 * \return None.
 *
 **/
static void I2CMasterControl(uint32_t baseAdd, uint32_t cmd)
{
   REG32(baseAdd + I2C_CON) = cmd | I2C_CON_I2C_EN;
}

/**
 * \brief   This API start's a I2C transaction on the bus. This API must
 *          be called after all the configuration for the i2c module is
 *          done and after bringing I2C out of local reset
 *
 * \param   baseAdd  It is the Memory address of the I2C instance used.
 *
 * \return None.
 **/
static void I2CMasterStart(uint32_t baseAdd)
{
   REG32(baseAdd + I2C_CON) |= I2C_CON_STT;
}

/**
 * \brief  This API stops a I2C transaction on the bus.
 *         This API must be used in case a deliberate STOP needs to be sent
 *         on the bus.
 *
 * \param  baseAdd  It is the Memory address of the I2C instance used.
 *
 * \return None.
 **/
static void I2CMasterStop(uint32_t baseAdd)
{
   REG32(baseAdd + I2C_CON) |= I2C_CON_STP;
}

/**
 * \brief  This API enables only specified I2C interrupts in master mode.
 *
 * \param  baseAdd  It is the Memory address of the I2C instance used.
 * \param  intFlag   It specifies the interrupts that are rquired to be enabled.\n
 *
 *         intFlag can take following values.\n
 *
 *         I2C_INT_ARBITRATION_LOST     - Arbitration-lost interrupt.\n
 *         I2C_INT_NO_ACK               - No-acknowledgment interrupt.\n
 *         I2C_INT_ADRR_READY_ACESS     - I2C registers are ready to access.\n
 *         I2C_INT_RECV_READY           - Receive-data-ready interrupt.\n
 *         I2C_INT_TRANSMIT_READY       - Transmit-data-ready interrupt.\n
 *         I2C_INT_GENERAL_CALL         - General call interrupt.\n
 *         I2C_INT_START                - Start condition interrupt.\n
 *         I2C_INT_ACCESS_ERROR         - Access error interrupt.\n
 *         I2C_INT_STOP_CONDITION       - Stop condition interrupt.\n
 *         I2C_INT_ADRR_SLAVE           - Address-as-slave interrupt.\n
 *         I2C_INT_TRANSMIT_UNDER_FLOW  - Transmit under flow interrupt.\n
 *         I2C_INT_RECV_OVER_RUN        - Receive overrun interrupt.\n
 *         I2C_INT_RECV_DRAIN           - Receive drain interrupt.\n
 *         I2C_INT_TRANSMIT_DRAIN       - Transmit drain interrupt.\n
 *
 * \return None.
 **/
static void I2CMasterIntEnableEx(uint32_t baseAdd, uint32_t intFlag)
{
   REG32(baseAdd + I2C_IRQENABLE_SET) |= intFlag;
}


/**
 * \brief  This API disables only specified I2C interrupts in master mode.
 *
 * \param  baseAdd  It is the Memory address of the I2C instance used.
 * \param  intFlag   It specifies the interrupts that are rquired to be disabled\n
 *
 *         intFlag can take following values.\n
 *
 *         I2C_INT_ARBITRATION_LOST     - Arbitration-lost interrupt.\n
 *         I2C_INT_NO_ACK               - No-acknowledgment interrupt.\n
 *         I2C_INT_ADRR_READY_ACESS     - I2C registers are ready to access.\n
 *         I2C_INT_RECV_READY           - Receive-data-ready interrupt.\n
 *         I2C_INT_TRANSMIT_READY       - Transmit-data-ready interrupt.\n
 *         I2C_INT_GENERAL_CALL         - General call interrupt.\n
 *         I2C_INT_START                - Start condition interrupt.\n
 *         I2C_INT_ACCESS_ERROR         - Access error interrupt.\n
 *         I2C_INT_STOP_CONDITION       - Stop condition interrupt.\n
 *         I2C_INT_ADRR_SLAVE           - Address-as-slave interrupt.\n
 *         I2C_INT_TRANSMIT_UNDER_FLOW  - Transmit under flow interrupt.\n
 *         I2C_INT_RECV_OVER_RUN        - Receive overrun interrupt.\n
 *         I2C_INT_RECV_DRAIN           - Receive drain interrupt.\n
 *         I2C_INT_TRANSMIT_DRAIN       - Transmit drain interrupt.\n
 *
 * \return None.
 **/
static void I2CMasterIntDisableEx(uint32_t baseAdd, uint32_t intFlag)
{
   REG32(baseAdd + I2C_IRQENABLE_CLR) = intFlag;
}

/**
 * \brief  This API returns the status of  interrupts in
 *         master mode.
 *
 * \param  baseAdd  It is the Memory address of the I2C instance used.
 *
 * \returns status of interrupts.
 **/
uint32_t I2CMasterIntStatus(uint32_t baseAdd)
{
   return ((REG32(baseAdd + I2C_IRQSTATUS)));
}

/**
 * \brief  This API Clears the status of specified interrupts in
 *         master mode.
 *
 * \param  baseAdd  It is the Memory address of the I2C instance used.
 * \param  intFlag  It specifies the interrupts whose status needs to be cleared\n
 *
 *         intFlag can take following macros.\n
 *
 *         I2C_INT_ARBITRATION_LOST     - Arbitration-lost interrupt.\n
 *         I2C_INT_NO_ACK               - No-acknowledgment interrupt.\n
 *         I2C_INT_ADRR_READY_ACESS     - I2C registers ready to access.\n
 *         I2C_INT_RECV_READY           - Receive-data-ready interrupt.\n
 *         I2C_INT_TRANSMIT_READY       - Transmit-data-ready interrupt.\n
 *         I2C_INT_GENERAL_CALL         - General call interrupt.\n
 *         I2C_INT_START                - Start condition interrupt.\n
 *         I2C_INT_ACCESS_ERROR         - Access error interrupt.\n
 *         I2C_INT_STOP_CONDITION       - Stop condition interrupt.\n
 *         I2C_INT_ADRR_SLAVE           - Address-as-slave interrupt.\n
 *         I2C_INT_TRANSMIT_UNDER_FLOW  - Transmit under flow interrupt.\n
 *         I2C_INT_RECV_DRAIN           - Receive drain interrupt.\n
 *         I2C_INT_TRANSMIT_DRAIN       - Transmit drain interrupt.\n
 *
 * \returns None
 **/
static void I2CMasterIntClearEx(uint32_t baseAdd, uint32_t intFlag)
{
   REG32(baseAdd + I2C_IRQSTATUS) = intFlag;
}

/**
 * \brief   This API sets the address of the slave device with which I2C wants to
 *          communicate.
 *
 * \param   baseAdd   It is the Memory address of the I2C instance used.
 * \param   slaveAddr  slave address.
 *
 * \return None.
 **/
static void I2CMasterSlaveAddrSet(uint32_t baseAdd, uint32_t slaveAdd)
{
   /*Set the address of the slave with which the master will communicate.*/
   REG32(baseAdd + I2C_SA) = slaveAdd;
}

/**
 * \brief   This API configure I2C data count register with a value.
 *          The value in the I2C data count register indicate how many data
 *          words to transfer when the I2C is configured as a  master-transmitter
 *          and repeat mode is off.
 *
 * \param   baseAdd  It is the Memory address of the I2C instance used.
 * \param   count    It is value which is set to I2C data count register.
 *
 * \return None.
 **/
static void I2CSetDataCount(uint32_t baseAdd, uint32_t count)
{
   REG32(baseAdd + I2C_CNT) = count;
}

/**
 * \brief   This API configure I2C data count register with a value.
 *          The value in the I2C data count register indicate how many data
 *          words to transfer when the I2C is configured as a  master-transmitter
 *          and repeat mode is off.
 *
 * \param   baseAdd  It is the Memory address of the I2C instance used.
 *
 * \return  number of bytes transfered over the I2C bus.
 **/
static uint32_t I2CDataCountGet(uint32_t baseAdd)
{
   return(REG32(baseAdd + I2C_CNT));
}

/**
 * \brief   This API reset the entire I2C module.On reset,are set to power
 *          up reset values.
 *
 * \param   baseAdd  It is the Memory address of the I2C instance used.
 *
 * \returns None.
 *
 **/
static void I2CSoftReset(uint32_t baseAdd)
{
   REG32(baseAdd + I2C_SYSC) |= I2C_SYSC_SRST;
}

/**
 * \brief   This API disables autoidle mechanism
 *
 * \param   baseAdd  It is the Memory address of the I2C instance used.
 *
 * \returns None.
 *
 **/
static void I2CAutoIdleDisable(uint32_t baseAdd)
{
   REG32(baseAdd + I2C_SYSC) &= ~I2C_SYSC_AUTOIDLE;
}

/**
 * \brief   This API indicates the state of the reset in case of
 *          hardware reset,global reset or paratial reset.
 *
 * \param   baseAdd  It is the Memory address of the I2C instance used.
 *
 * \returns returns "1" if reset is completed.
 *          returns "0" if internal module reset is ongoing.
 *
 **/
static uint32_t I2CSystemStatusGet(uint32_t baseAdd)
{
   return(REG32(baseAdd + I2C_SYSS) & I2C_SYSS_RDONE);
}

/**
 * \brief  This function Transmits a byte from the I2C in Master mode.
 *
 * \param  baseAdd is the Memory address of the I2C instance used.
 * \param  data data to be transmitted from the I2C Master.
 *
 * \return None.\n
 *
 **/
static void I2CMasterDataPut(uint32_t baseAdd, uint8_t data)
{
   /*write data to be transmited to Data transmit register */
   REG32(baseAdd + I2C_DATA) = data;
}

/**
 *  \brief  This Receives a byte that has been sent to the I2C in Master mode.
 *
 *  \param  baseAdd is the Memory address of the I2C instance used.
 *
 *  \return Returns the byte received from by the I2C in Master mode.
 *
 **/
static uint8_t I2CMasterDataGet(uint32_t baseAdd)
{
   return (uint8_t)REG32(baseAdd + I2C_DATA);
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
